{
   "application" : {  
      // user specific data  
      "connection_string" : "mysql:database=oastat",  
      "media_path" : "../../files/media" 
   },  
    "service" : {
        "api" : "http",
        "port" : 27960
    },
    "http" : {
        "script_names" :  ["/oastatweb"]
    },
    "file_server" : {
        "enable": true,
	"document_root" : ".",
	"alias" : [  
    		{ "url" : "/files/media" , "path" : "./media" },  
  	],  
    }
}

