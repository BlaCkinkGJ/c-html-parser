# HTML TIDY EXAMPLE

This program based on [this example](https://curl.haxx.se/libcurl/c/htmltidy.html).
You can use this program like below.

```bash
make clean
make
./htmltidy https://www.google.com
```

Sometimes you probably can encounter the error like 301 or 302.
For example, I type the command like below.

```bash
./htmltidy google.com
```

Then this program shows like below.

```html

* Rebuilt URL to: https://google.com/
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed

  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0*   Trying 172.217.161.142...
* TCP_NODELAY set
* Connected to google.com (172.217.161.142) port 443 (#0)
* ALPN, offering http/1.1
* successfully set certificate verify locations:
*   CAfile: /etc/ssl/certs/ca-certificates.crt
  CApath: /etc/ssl/certs
* SSL connection using TLSv1.3 / TLS_AES_256_GCM_SHA384
* ALPN, server accepted to use http/1.1
* Server certificate:
*  subject: C=US; ST=California; L=Mountain View; O=Google LLC; CN=*.google.com
*  start date: Feb 12 11:47:11 2020 GMT
*  expire date: May  6 11:47:11 2020 GMT
*  subjectAltName: host "google.com" matched cert's "google.com"
*  issuer: C=US; O=Google Trust Services; CN=GTS CA 1O1
*  SSL certificate verify ok.
> GET / HTTP/1.1
Host: google.com
Accept: */*

< HTTP/1.1 301 Moved Permanently
< Location: https://www.google.com/
< Content-Type: text/html; charset=UTF-8
< Date: Wed, 19 Feb 2020 02:44:54 GMT
< Expires: Fri, 20 Mar 2020 02:44:54 GMT
< Cache-Control: public, max-age=2592000
< Server: gws
< Content-Length: 220
< X-XSS-Protection: 0
< X-Frame-Options: SAMEORIGIN
< Alt-Svc: quic=":443"; ma=2592000; v="46,43",h3-Q050=":443"; ma=2592000,h3-Q049=":443"; ma=2592000,h3-Q048=":443"; ma=2592000,h3-Q046=":443"; ma=2592000,h3-Q043=":443"; ma=2592000
< 

100   220  100   220    0     0   1235      0 --:--:-- --:--:-- --:--:--  1235
* Connection #0 to host google.com left intact
P
(null)
html PUBLIC >
html >
   <head >
       <meta name="generator" content="HTML Tidy for HTML5 for Linux version 5.2.0" >
       <meta http-equiv="content-type" content="text/html; charset=utf-8" >
       <title >
  301 Moved

   <body >
       <h1 >
  301 Moved

The docu
       <a href="https://www.google.com/" >
       here

      .
```

This means that `google.com` doesn't correct URL. So, you have to change that value to `https://www.google.com` which locates in `< Location: ~~~`.

NOTICE) Current version doesn't support the other languages setting except english.
