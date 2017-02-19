{
   "application" : {
      // user specific data
      "connection_string" : "mysql:database=oastat;user=testuser;password='password'",
      "media_path" : "../../files/media",
      "static_media" : "../../files/static"
   },
    "service" : {
        "api" : "http",
        "port" : 27960
    },
    "http" : {
  "script" : "/mb.fcgi",
  "rewrite" : [
    { "regex" : "/media(/.*)?", "pattern" : "$0" },
    { "regex" : "/favicon\\.ico", "pattern" : "$0" },
    { "regex" : ".*" , "pattern" : "/mb.fcgi$0" }
  ]
}  ,
    "file_server" : {
        "enable": true,
	"document_root" : ".",
	"alias" : [
    		{ "url" : "/files/media" , "path" : "./media" },
		{ "url" : "/files/static/" ,"path" : "./static_media"},
  	],
    }
}
