#include<iostream>
#include<bits/stdc++.h>
#include<map>
#include<forward_list>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#ifdef _WIN32
#include<windows.h>
#endif
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif 


//for linux it is <arpa/inet.h> and <sys/socket.h>
using namespace std; 
// amit [Bro programmer]
class BroUtilities{
	BroUtilities(){}
	public:
		
		static bool isHexChar(int w)
		{
			if(w>=48 && w<=57) return true;
			if(w>='a' && w<='f') return true;
			if(w>='A' && w<='F') return true;
			return false;
		}
		
		static void decode(char *encodedString, char *decodedString)
		{
			char *ptr=encodedString;
			char *d=decodedString;
			int i,m;
			i=0;
			while(*ptr!='\0')
			{
				if(*ptr=='+')
				{
					d[i]=' ';
					i++;
					ptr++;
					continue;
				}
				if(*ptr!='%')
				{
					d[i]=*ptr;
					i++;
					ptr++;
					continue;
				}
				ptr++;
				if(isHexChar(*ptr) && isHexChar(*(ptr+1)))
				{
					sscanf(ptr,"%2x",&m);
					d[i]=m;
					i++;
					ptr+=2;
				}
				else
				{
					i=0;
					break;
				}
			}
			d[i]='\0';
		}
		
		static void loadMIMETypes(map<string,string> &mimeTypesMap)
		{
			FILE *file;
			file=fopen("brodata/mime.types","r");
			if(file==NULL) return;
			char *mimeType;
			char *extension;
			char line[200];
			int x;
			while(true)
			{
				fgets(line,200,file);
				if(feof(file)) break;
				if(line[0]=='#') continue;
				//logic to remove \r \n from the end of the string starts here
				x=strlen(line)-1;
				while(true)  
				{
					if(line[x]=='\r' || line[x]=='\n')
					{
						line[x]='\0';
						x--;
					}
					else{
						break;
					}
					
				}
				//logic to remove \r \n from the end of the string ends here
				mimeType=&line[0];
				for(x=0;line[x]!='\t';x++);
				line[x]='\0';
				x++;
				while(line[x]=='\t') x++;
				while(true)
				{
					extension=&line[x];
					while(line[x]!='\0' && line[x]!=' ') x++;
					if(line[x]=='\0')
					{
						//add entry to map and break the loop
						mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
						//cout<<extension<<"	 ,	 "<<mimeType<<endl;
						break;
					}
					else{
						// place \0 on the xth index, add entry to map and increment the value of x
						line[x]='\0';
						mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
						//cout<<extension<<"	 ,	 "<<mimeType<<endl;
					}
				} // parsing extensions
			}
			fclose(file);
		}
		
		
	
};

class FileSystemUtility{
	private:
		FileSystemUtility() {}
	public:
		static bool fileExists(const char *path)
		{	
			struct stat s;
			int x;
			x=stat(path,&s);
			if(x!=0) return false;
			if(s.st_mode & S_IFDIR) return false; //0 : file exists
			return true;
			
		}
		static bool directoryExists(const char *path)
		{
			struct stat s;
			int x;
			x=stat(path,&s);
			if(x!=0) return false;
			if(s.st_mode & S_IFDIR) return true;
			return false;
		}
		static string getFileExtension(const char *path)
		{
			int x;
			x=strlen(path)-1;
			while(x>=0 && path[x]!='.') x--;
			if(x==-1 || path[x]!='.') return string("");
			return string(path+(x+1));
		}
	
};

class StringUtility{
	private:
		StringUtility();
	public:
		static void toLowerCase(char *str)
		{
			if(str==NULL) return;
			for(;*str;str++) if(*str>=65 && *str<=90) *str+=32;
		}
};
class HttpErrorStatusUtility
{
	private:
		HttpErrorStatusUtility() {
		}
	public:
	static void sendBadRequestError(int clientSocketDescriptor)
	{
		//later
	}
	static void	sendHttpVersionNotSupportedError(int clientSocketDescriptor, char *httpVersion)
	{
		//later
	}
	static void sendNotFoundError(int clientSocketDescriptor,char *requestURI)
	{       // optimize it later
			char content[1000];
			char response[1200];
			char header[200];
	sprintf(content,"<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>404 Error</title></head><body>Requested Resource [%s] Not found</body></html>",requestURI);
	int contentLength=strlen(content);
	sprintf(header,"HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n", contentLength);
	strcpy(response,header);
	strcat(response,content);
	send(clientSocketDescriptor,response,strlen(response),0);
				
	}
	static void sendMethodNotAllowedError(int clientSocketDescriptor,char *method, char *requestURI)
	{
		//later
	}
	
};
class Validator{
	public:
		static bool isValidMIMEType(string &mimeType)
		{
			//nothing right now
			return true;
		}
		static bool isValidPath(string &path)
		{
			return FileSystemUtility::directoryExists(path.c_str());
		}
		static bool isValidURLFormat(string &url)
		{
			//nothing right now
			return true;
		}
};
class Error{
	string error;
	public:
		Error(string error)
		{
			this->error=error;
		}
		bool hasError()
		{
			return this->error.length()>0;
		}
		string getError()
		{
			return "";
		}
};
class Request{
	private:
		char *method;
		char *requestURI;
		char *httpVersion;
		map<string,string> dataMap;
		Request(char *method, char *requestURI, char *httpVersion, char *dataInRequest)
		{
			this->method=method;
			this->requestURI=requestURI;
			this->httpVersion=httpVersion;
			if(dataInRequest!=NULL && strcmp(this->method,"get")==0)
			{
				createDataMap(dataInRequest,dataMap);
			}
		}
		
		void createDataMap(char *str,map<string,string> &dataMap)
{
	char *ptr1,*ptr2;
	while(1)
	{
		char *ptr1, *ptr2;
		char *decoded;
		int keyLength, valueLength;
		ptr1=str;
		ptr2=str;
		while(1)
		{
			while(*ptr2!='\0' && *ptr2!='=') ptr2++;
			if(*ptr2=='\0') return;
			*ptr2='\0';
			keyLength=ptr2-ptr1;
			decoded = new char[keyLength+1];
			BroUtilities::decode(ptr1,decoded);
			
			string key=string(decoded);
			delete [] decoded;
			ptr1=ptr2+1;
			ptr2=ptr1;
			while(*ptr2!='\0' && *ptr2!='&') ptr2++;
			if(*ptr2=='\0')
			{
				valueLength=ptr2-ptr1;
				decoded=new char[valueLength+1];
				BroUtilities::decode(ptr1,decoded);
				dataMap.insert(pair<string,string>(key,string(decoded)));
				delete [] decoded;
				break;
			}
			else
			{
				*ptr2='\0';
				valueLength=ptr2-ptr1;
				decoded=new char[valueLength+1];
				BroUtilities::decode(ptr1,decoded);				
				dataMap.insert(pair<string,string>(key,string(decoded)));
				delete [] decoded;
				ptr1=ptr2+1;
				ptr2=ptr1;
			}
			
		}
	}//end if infinite loop
}
		public:
			string operator[](string key)
			{
				auto iterator=dataMap.find(key);
				if(iterator==dataMap.end())
				{ 
					return string("");
				}
				return iterator->second;
				
			}
			
		
		
		friend class Bro;
};
class Response{
	string contentType;
	forward_list<string> content;
	forward_list<string>::iterator contentIterator;
	unsigned long contentLength;
	public:
		Response()
		{
			this->contentLength=0;
			this->contentIterator=this->content.before_begin();
		}
		~Response()
		{
			//not yet decided
		}
		void setContentType(string contentType)
		{   if(Validator::isValidMIMEType(contentType)){
				this->contentType=contentType;
		}
		
		}
		Response & operator<<(string content)
		{
			this->contentLength+=content.length();
			this->contentIterator=this->content.insert_after(this->contentIterator,content);
			return *this;
		}
		friend class HttpResponseUtility;
};
class HttpResponseUtility{
	private:
		HttpResponseUtility() {}
	public:
		
	static void sendResponse(int clientSocketDescriptor,Response &response)
	{

			char header[200];
	
	int contentLength=response.contentLength;
	sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n", contentLength);
	send(clientSocketDescriptor,header,strlen(header),0);
	auto contentIterator=response.content.begin();
	while(contentIterator!=response.content.end())
	{
		string str=*contentIterator;
		send(clientSocketDescriptor,str.c_str(),str.length(),0);
		++contentIterator;
	}
	}
	
	
};




// instead of a string use enum data
enum __request_method__{
__GET__, __POST__ , __PUT__ ,
__DELETE__ , __CONNECT__ , __TRACE__,
__HEAD__ , __OPTIONS__
};
typedef struct __url__mapping
{
	__request_method__	requestMethod;
	void (*mappedFunction)(Request &, Response &);
}URLMapping;
/*
struct __url__mapping : URLMapping is structure of type __url__mapping 
typedef struct __url__mapping : URLMapping is an alias of __url__mapping
*/
class Bro{
	string staticResourcesFolder;
	map<string, URLMapping> urlMappings;
	map<string, string> mimeTypes;
//	map<string,void (*)(Request &, Response &)> urlMappings;
	public:
		Bro()
		{
			BroUtilities::loadMIMETypes(mimeTypes);
			if(mimeTypes.size()==0) throw string("brodata folder has been tampered with");
		}
		~Bro()
		{
			// not 
		}
		 void setStaticResourcesFolder(string staticResourcesFolder)
		 {
		 	if(Validator::isValidPath(staticResourcesFolder))
		 	{
		 		this->staticResourcesFolder=staticResourcesFolder;
			 }
			 else
			 {
			 	string exception=string("Invalid static resource folder path :")+staticResourcesFolder;
			 	throw exception;
			 }
		 }  
		 bool serveStaticResource(int clientSocketDescriptor, const char *requestURI)
		 {
			if(this->staticResourcesFolder.length()==0) return false;
			if(!FileSystemUtility::directoryExists(this->staticResourcesFolder.c_str())) return false;
			string resourcePath=this->staticResourcesFolder+string(requestURI);
			//cout<<"Static resource path is "<<resourcePath<<endl;
			if(!FileSystemUtility::fileExists(resourcePath.c_str())) return false;
			FILE *file=fopen(resourcePath.c_str(),"rb");
			if(file==NULL) return false;
			long fileSize;
			fseek(file,0,SEEK_END);
			fileSize=ftell(file);
			if(fileSize==0)
			{
				fclose(file);
				return false;
			}
			rewind(file); // to move the internal file pointer to the start of the file
			
			//extract mime type
			string extension,mimeType;
			extension=FileSystemUtility::getFileExtension(resourcePath.c_str());
			if(extension.length()>0)
			{	
				transform(extension.begin(),extension.end(),extension.begin(),::tolower);
				auto mimeTypesIterator=mimeTypes.find(extension);
				if(mimeTypesIterator!=mimeTypes.end())
				{
					mimeType=mimeTypesIterator->second;
				}
				else
				{
					mimeType=string("text/html");
				}
			}
			else
			{
				mimeType=string("text/html");
			}
			//cout<<resourcePath<<" , "<<extension<<" , "<<mimeType<<endl;
			char header[200];
			//sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",fileSize);
			sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",mimeType.c_str(),fileSize);
			send(clientSocketDescriptor,header,strlen(header),0);
			long bytesLeftToRead;
			int bytesToRead;
			char buffer[4096];
			bytesToRead=4096;
			bytesLeftToRead=fileSize;
			while(bytesLeftToRead>0)
			{
				if(bytesLeftToRead<bytesToRead) bytesToRead=bytesLeftToRead;
				fread(buffer,bytesToRead,1,file);
				if(feof(file)) break; //this won't happen in this case
				send(clientSocketDescriptor,buffer,bytesToRead,0);
				bytesLeftToRead=bytesLeftToRead-bytesToRead;
			}
			fclose(file);
			return true;
			
		 }
		 void get(string url,void (*callBack)(Request &, Response &))
		 {
		 	if(Validator::isValidURLFormat(url))
		 	{/* for older versions 
				URLMapping u;
				u.requestMethod=__GET__;
				u.mappedFunction=callBack;
				
				URLMapping u = {__GET__, callback};
				
				urlMappings.insert(pair<string,URLMapping>(url,u));			 
			 
			 */
		 		urlMappings.insert(pair<string,URLMapping>(url,{__GET__,callBack}));
		 	 //	urlMappings.insert(pair<string,void (*)(Request &,Response &)>(url,callBack));
			 }
		 }
		 void post(string url,void (*callBack)(Request &, Response &))
		 {
		 	if(Validator::isValidURLFormat(url))
		 	{
		 		urlMappings.insert(pair<string,URLMapping>(url,{__POST__,callBack}));
		 	 
			 }
		 }
		 void listen(int portNumber,void (*callBack)(Error &))
		 {  WSADATA wsaData;
		 	WORD ver;
		 	ver=MAKEWORD(1,1);
		 	WSAStartup(ver,&wsaData);
		 
		    int serverSocketDescriptor;
		    char requestBuffer[4097]; // 4096+1 : 1 byte extra for '/0'
		    int requestLength;
		    int x;
		 	serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			if(serverSocketDescriptor<0)
			{   WSACleanup();
				Error error("Unable to create socket");
				callBack(error);
				return;
			  }  
			  struct sockaddr_in serverSocketInformation; 
			  serverSocketInformation.sin_family=AF_INET;
			  serverSocketInformation.sin_port=htons(portNumber);
			  serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
			  int successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));			  
		 	  if(successCode<0)
		 	  {
		 	  	close(serverSocketDescriptor);
		 	  	WSACleanup();
		 	  	char a[101];
		 	  	sprintf(a,"Unable to bind socket to port: %d",portNumber);
		 	  	Error error(a);
		 	  	callBack(error);
		 	  	return;
			   }
			   successCode= ::listen(serverSocketDescriptor,10);
			   if(successCode<0)
			   {
			   	close(serverSocketDescriptor);
			   	WSACleanup();
			   Error error("Unable to accept client connections");
			   callBack(error);
			   return;
		        }
		        Error error("");
		        callBack(error);
		        struct sockaddr_in clientSocketInformation;
		        //linux: socklen_t
				int len=sizeof(clientSocketInformation);
				//int requestNumber=0;
		        int clientSocketDescriptor;
		        while(1)
		        {
		        	clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
		        	//to check that there is a bug
		        	
		        	
		        	// remove from here 
				requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
				if(requestLength==0 || requestLength==-1)
					//uptill here
				{
							close(clientSocketDescriptor);
							continue;
						}
						int i;
						char *method, *requestURI, *httpVersion,*dataInRequest;
						requestBuffer[requestLength]='\0';
						
						//code to parse the first line of the http request starts here
						//first line should be REQUEST_METHOD SPACE URI SPACE HTTPVersionCRLF
						
						method=requestBuffer;
						i=0;
						while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
						if(requestBuffer[i]=='\0')
						{
							HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
							close(clientSocketDescriptor);
							continue;
						}
						requestBuffer[i]='\0';
						i++;
						if(requestBuffer[i] == ' ' || requestBuffer[i]=='\0')
						{
							HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
							close(clientSocketDescriptor);
							continue;
						}
						StringUtility::toLowerCase(method);
						
						if(!(strcmp(method,"get")==0 ||
						strcmp(method,"post")==0 ||
						strcmp(method,"put")==0 ||
						strcmp(method,"delete")==0 ||
						strcmp(method,"head")==0 ||
						strcmp(method,"options")==0 ||
						strcmp(method,"trace")==0 ||
						strcmp(method,"connect")==0))
						{
							HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
							close(clientSocketDescriptor);
							continue;
						}
						requestURI=requestBuffer+i;
						while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
						if(requestBuffer[i]=='\0')
						{
							HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
							close(clientSocketDescriptor);
							continue;
						}
						requestBuffer[i]='\0';
						i++;
						if(requestBuffer[i]==' ' || requestBuffer[i]=='\0')
						{
							HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
							close(clientSocketDescriptor);
							continue;
						}
						
						httpVersion=requestBuffer+i;
						while(requestBuffer[i] && requestBuffer[i] && requestBuffer[i]!='\r' && requestBuffer[i]!='\n') i++;
						if(requestBuffer[i]=='\0')
						{
							HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
							close(clientSocketDescriptor);
							continue;
						}
						if(requestBuffer[i]=='\r' && requestBuffer[i+1]!='\n')
						{
							HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
							close(clientSocketDescriptor);
							continue;
						}
						if(requestBuffer[i]=='\r')
						{
							requestBuffer[i]='\0';
							i=i+2;
						}
						else
						{
							requestBuffer[i]='\0';
							i=i+1;
						}
						StringUtility::toLowerCase(httpVersion);
						if(strcmp(httpVersion,"http/1.1")!=0)
						{
							HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor,httpVersion);
							close(clientSocketDescriptor);
							continue;
						}
						dataInRequest=NULL;
						i=0;
						while(requestURI[i]!='\0' && requestURI[i]!='?') i++;
						if(requestURI[i]=='?')
						{
							requestURI[i]='\0';
							dataInRequest=requestURI+i+1;
						}
						
						//cout<<"Request arrived, uri is "<<requestURI<<endl;
						
						auto urlMappingsIterator=urlMappings.find(requestURI);
						if(urlMappingsIterator==urlMappings.end())
						{
							if(!serveStaticResource(clientSocketDescriptor,requestURI))
							{
							HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
							}
							close(clientSocketDescriptor);
							continue;
						}
						URLMapping urlMapping=urlMappingsIterator->second;
						if(urlMapping.requestMethod==__GET__ and strcmp(method,"get")!=0)
						{
							HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,method,requestURI);
							close(clientSocketDescriptor);
							continue;
						}
						
						//code to parse the header and then the payload if exists starts here
						//code to parse the header and then the payload if exists ends here
						Request request(method,requestURI, httpVersion,dataInRequest);
						Response response;
						urlMapping.mappedFunction(request,response);
						HttpResponseUtility::sendResponse(clientSocketDescriptor,response);
						
						close(clientSocketDescriptor);
					//lot of code will be here
				} // infinite loop ends here
		       WSACleanup();
		 }
		 
};




// Bob [Web application developer - user of a web server]
int main()
{	try{
	Bro bro;
	bro.setStaticResourcesFolder("whatever");
	bro.get("/save_test1_data",[](Request &request, Response &response) {
	string nnn=request["nm"];
	string ccc=request["ct"];
	cout<<"Data that arrived in request"<<endl;
	cout<<nnn<<endl;
	cout<<ccc<<endl;
	cout<<"---------------------------------"<<endl;
	const char *html=R""""(
	<!DOCTYPE HTML>
	<html lang='en'>
	<head>
	<meta charset='utf-8'>
	<title>BRO Test Cases</title>
	</head>
	<body>
	<h1>Test Case 1 - GET with Query String</h1>
	<h3>Response From Server Side</h3>
	<b>Data Saved</b>
	<br/><br/>
	<a href='/testindex.html'>Home</a>  
	</body>
	</html>
	)"""";
	response.setContentType("text/html");
	response<<html;
	});
	
	bro.post("/save_test2_data",[](Request &request, Response &response) {
	const char *html=R""""(
	<!DOCTYPE HTML>
	<html lang='en'>
	<head>
	<meta charset='utf-8'>
	<title>BRO Test Cases</title>
	</head>
	<body>
	<h1>Test Case 2 - POST with Form Data</h1>
	<h3>Response From Server Side</h3>
	<b>Data Saved</b>
	<br/><br/>
	<a href='/testindex.html'>Home</a>  
	</body>
	</html>
	)"""";
	response.setContentType("text/html");
	response<<html;
	});
	
	bro.get("/getCustomers",[](Request &request, Response &response) {
	const char *html=R""""(
	<!DOCTYPE HTML>
	<html lang='en'>
	<head>
	<meta charset='utf-8'>
	<title>Whatever</title>	
	</head>
	<body>
	<h1>List of Customers</h1>
	<ul>
	<li>Anadi</li>
	<li>Akshu</li>
	<li>Aditi</li>
	</ul>
	<a href='/'>Home</a>
	</body>
	</html>
	)"""";
	
	response.setContentType("text/html"); // setting mime type
	response<<html;
	});
	
	bro.listen(6060,[](Error &error) {
		if(error.hasError())
		{
			cout<<error.getError();
			return;
		}
		cout<<"Bro HTTP Server is ready to accept request on port 6060"<<endl;
	});
	
}catch(string exception)

{
	cout<<exception<<endl;
	
	/*It is code to verify that whatever the
	 web server user is specifying exists and if
	 it is not then raise an exception*/
	 
}

	return 0;
}
