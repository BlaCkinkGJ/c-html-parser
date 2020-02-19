#include <curl/curl.h>
#include <stdio.h>
#include <tidy.h>
#include <tidybuffio.h>

//////////////////// MACRO SECTION ////////////////////
#define TIDY_INT_MAX (4096)
#define IS_CURL_HAS_ERROR(err) (err)
#define IS_TIDY_HAS_ERROR(err) (err > 0) /* 0: Normal, 1: Warning, 2: Error */
#define IS_TIDY_HAS_VALID_CODE(err) (err >= 0)

//////////////////// DECLARATION SECTION ////////////////////
uint write_callback(char *inp_buffer, uint size, uint nr_memory_buffer,
		    TidyBuffer *outp_buffer);
void dump_node(TidyDoc doc, TidyNode tidy_node, int indent);

//////////////////// MAIN SECTION ////////////////////
int main(int argc, char **argv)
{
	CURL *curl;

	char curl_error_buffer[CURL_ERROR_SIZE];

	TidyDoc tidy_doc;
	TidyBuffer doc_buffer = { 0 };
	TidyBuffer tidy_err_buffer = { 0 };

	int curl_err = 0, tidy_err = 0;

	if (argc == 2) {
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL,
				 argv[1]); /* Get URL from argument */
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(
			curl, CURLOPT_WRITEFUNCTION,
			write_callback); /* write callback function setting */

		tidy_doc = tidyCreate();
		tidyOptSetBool(tidy_doc, TidyForceOutput, yes);
		tidyOptSetInt(tidy_doc, TidyWrapLen, TIDY_INT_MAX);
		tidySetErrorBuffer(tidy_doc, &tidy_err_buffer);
		tidyBufInit(&doc_buffer);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &doc_buffer);
		curl_err = curl_easy_perform(curl);
		if (IS_CURL_HAS_ERROR(curl_err)) {
			goto exception;
		}

		tidy_err = tidyParseBuffer(tidy_doc, &doc_buffer);
		if (!IS_TIDY_HAS_VALID_CODE(tidy_err)) {
			goto exception;
		}

		tidy_err = tidyCleanAndRepair(tidy_doc);
		if (!IS_TIDY_HAS_VALID_CODE(tidy_err)) {
			goto exception;
		}

		tidy_err = tidyRunDiagnostics(tidy_doc);
		if (!IS_TIDY_HAS_VALID_CODE(tidy_err)) {
			goto exception;
		}

		dump_node(tidy_doc, tidyGetRoot(tidy_doc), 0);
		if (IS_TIDY_HAS_ERROR(tidy_err)) {
			goto exception;
		}
	} else {
		printf("usage: %s <url>\n", argv[0]);
	} // end of if

exception:
	curl_easy_cleanup(curl);
	tidyBufFree(&doc_buffer);
	tidyBufFree(&tidy_err_buffer);
	tidyRelease(tidy_doc);

	if (IS_CURL_HAS_ERROR(curl)) {
		fprintf(stderr, "%s\n", curl_error_buffer);
	}
	if (IS_TIDY_HAS_ERROR(tidy_err)) {
		fprintf(stderr, "%s\n", tidy_err_buffer.bp);
	}

	return (curl_err | tidy_err);
}

//////////////////// DEFINITION SECTION ////////////////////
uint write_callback(char *inp_buffer, uint size, uint nr_memory_buffer,
		    TidyBuffer *outp_buffer)
{
	uint ret;
	ret = size * nr_memory_buffer;
	tidyBufAppend(outp_buffer, inp_buffer, ret);
	return ret;
}

void dump_node(TidyDoc doc, TidyNode tidy_node, int indent)
{
	TidyNode child;
	for (child = tidyGetChild(tidy_node); child;
	     child = tidyGetNext(child)) {
		ctmbstr name = tidyNodeGetName(child);
		if (name) { /* Case TAG Inforamtion*/
			TidyAttr attr;
			printf("%*.*s%s ", indent, indent, "<",
			       name); /* Ignore the `indent` size characters */
			for (attr = tidyAttrFirst(child); attr;
			     attr = tidyAttrNext(attr)) {
				printf("%s", tidyAttrName(attr));
				tidyAttrValue(attr) ?
					printf("=\"%s\" ",
					       tidyAttrValue(attr)) :
					printf(" ");
			} // end of for
			printf(">\n");
		} else { /* Case: Text Information*/
			TidyBuffer buf;
			tidyBufInit(&buf);
			tidyNodeGetText(doc, child, &buf);
			printf("%*.*s\n", indent, indent,
			       buf.bp ? (char *)buf.bp : " ");
			tidyBufFree(&buf);
		} // end of if
		dump_node(doc, child, indent + 4);
	} // end of for
}
