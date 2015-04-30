#define FUSE_USE_VERSION 26
#include <fstream>
#include <iostream>
#include <fuse.h>
#include <stdio.h>
#include <map>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <signal.h>
#include <time.h> 

using namespace std;

int ii = -1;
int socket_list[3];
int thread_port = 10200;
int port_array[3];
char password[20] = "ESA";
void *probe_connection_handler(void *);

typedef struct fs {
	int isdir;
	//string data;
	string file_name;
	int blockn;
	int size;	
	//struct stat attrib;
	//std::vector<std::string> insertOrder;
}fmeta;


typedef struct block {
	int size;
	int ref;
	int ncopy;
	//int start;
	//int end;
	string host1;
	string host2;

}blockmeta;


string replace(string s)
{
	std::replace( s.begin(), s.end(), '/', '_');
	return s;
}

char cw[120];
string cwd;

void *probe_connection_handler(void *sock_desc_v)
{	
	int sock_number;	
	int sock_desc = *(int *)sock_desc_v;
	int sock,new_socket_t;
	if (sock_desc_v==NULL)
	{
		printf("Error: Thread not assigned");
		//exit (0);
	}
	
	//password check

	char* str2p = (char *)malloc(sizeof (char)*100);
	memset(str2p, 0, 100);
	strcpy(str2p,"Please enter the Password");
	char* reply_str2p = (char *)malloc(sizeof (char)*100);
	memset(reply_str2p, 0, 100);
	char* freply_str2p = (char *)malloc(sizeof (char)*100);
	memset(freply_str2p, 0, 100);
	char* str3p = (char *)malloc(sizeof (char)*100);
	memset(str3p, 0, 100);
	strcpy(str3p,"Correct Passwordsss");
	char* ftr3p = (char *)malloc(sizeof (char)*100);
	memset(ftr3p, 0, 100);
	strcpy(ftr3p,"Correct Password");

	if (write(sock_desc,str2p,strlen(str2p)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}		

	if(read(sock_desc, reply_str2p, 100) < 0)
	{
		printf("Error: Received failed");
		return 0;
	}	
	if(strcmp(password,reply_str2p)!=0)
	{
		char* str21pp = (char *)malloc(sizeof (char)*100);
		memset(str21pp, 0, 100);
		strcpy(str21pp,"Wrong Password");
		if (write(sock_desc,str21pp,strlen(str21pp)) < 0)
		{
			printf("Error: Send failed");
			return 0;
		}
		close(sock_desc);
		printf("\nWrong Password");
		fflush(stdout);		
	}
	
	if (write(sock_desc,ftr3p,strlen(ftr3p)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}	
	
	if(read(sock_desc, freply_str2p, 100) < 0)
	{
		printf("Error: Received failed");
		return 0;
	}
	//printf("\nWaste received\n");
	socket_list[++ii] = sock_desc;	// global variable //
	printf ("\nConnection Accepted\n");
	socklen_t server_len_t,client_len;
	struct sockaddr_in serv_addr_t,client_addr_t;
	
	sock = socket (AF_INET, SOCK_STREAM, 0);
	
	if (sock == -1)
	{
		printf("\nError: Opening socket\n");
		perror("");
		return 0;
	}

	memset(&serv_addr_t, '0', sizeof(serv_addr_t));

	//Socket address assignment
	serv_addr_t.sin_family = AF_INET;
	serv_addr_t.sin_addr.s_addr = INADDR_ANY;
	serv_addr_t.sin_port = htons(thread_port);
	
	port_array[ii] = thread_port; 
	char *str;
	str = (char*)malloc(sizeof(char)*25);
	sprintf(str,"%d",thread_port);
	if (write(sock_desc,str,strlen(str)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}
	printf("\nPort No Sent\n");	

	//Bind socket with port
	if (bind(sock, (struct sockaddr *)&serv_addr_t, sizeof(serv_addr_t)) == -1)
	{
		printf("\nError: Binding not successful inside socket\n");
		perror("");		
		return 0;
	}
	printf("Bind successful\n");


	//Listening to connections
	if(listen(sock, 5) == -1)
	{
		printf("\nFailed to listen\n");
		return 0;
	}
	printf("Listening to port"); // this line exec whenever a new client tries to communicate.
	
	//main socket sending new socket info to client
	/*char *str;
	str = (char*)malloc(sizeof(char)*25);
	sprintf(str,"%d",thread_port);
	if (write(sock_desc,str,strlen(str)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}*/

	//Accepting connections from client.
	client_len = sizeof(client_addr_t);
	new_socket_t = accept(sock, (struct sockaddr *) &client_addr_t, &client_len);
	
	//sending and receiving probes
	char* reply_str = (char *)malloc(sizeof (char)*20);
	memset(reply_str, 0, 20);
	char* str1 = (char *)malloc(sizeof (char)*30);
	memset(str1, 0, 30);
	strcpy(str1,"Are you active?");
	thread_port++;
	int n =5000;
	while(1)
	{
		//put timer here
		
		
		if (write(new_socket_t,str1,strlen(str1)) < 0)
		{
			printf("Error: Send failed");
			return 0;
		}
		printf("\nSent\n");
		/////////////receiving reply from client////////////////////
		
		if(read(new_socket_t, reply_str, 10) < 0)
		{
			printf("Error: Received failed");
			return 0;
		}
		printf("\nReceived\n");
		
		if (strcmp(reply_str,"\0") == 0)
		{	
			printf("Client Disconnected.");
			break;
			//exit (0);
		}
		else if (strcmp(reply_str,"I am done") == 0)
		{
			close(new_socket_t);
			close(sock_desc); 
			printf("Client Disconnected by interrupt");
						
			//close(stdout);
			printf("\nSocket Closed");
			fflush(stdout);	
			break;
		}
		else
			printf("Client Connected");
		n--;
	}
	
	
}

void get_metaf(string path,fmeta*  c)
{
	cout <<"entered get metaf and the path is " << path <<endl;
	string p = replace (path) ;
	string file = cwd + "/meta/"+ p ;
	FILE *fp;
	fp = fopen(file.c_str(), "r");
	char line[120] = "";
	fread(line, 120, 1, fp);
	char fll[60];
	sscanf(line, "%d|%d|%d|%s\n", &(c->isdir),&(c->blockn),&(c->size),fll);
	c->file_name = string(fll);
	fclose(fp);
	return ;
	//sprintf(metaString, "%d|%d|%s\n", b.isdir, b.blockn,b.file_name.c_str());
}


void write_metab(string path,blockmeta*  b)
{
	FILE *meta,*fp;
	char metaString[160];
	cout <<"entered the write metab and the path is "<< path <<endl; 
	sprintf(metaString, "%d|%d|%d|%s\n", b->size, b->ref,b->ncopy,b->host1.c_str());
	
	meta = fopen(path.c_str(), "w");
	fwrite(metaString, 160, 1, meta);
	fclose(meta);
}

void get_metab(string path, blockmeta* c)
{
	//string p = replace (path);
	//string file = cwd +"/block/" + p;
	FILE *fp;
	fp = fopen(path.c_str(), "r");
	char line[160] = "";
	fread(line, 160, 1, fp);
	char fll[80];
	sscanf(line, "%d|%d|%d|%s\n", &(c->size),&(c->ref),&(c->ncopy),fll);
	c->host1 = string(fll);
	return ;
	
}
void write_metaf(string path, fmeta*  c)
{

	string p = replace (path) ;
	string file = cwd + "/meta/"+ p ;       
	FILE *fp;
	cout << "entered the write meta file " << file << endl ;
        fp = fopen(file.c_str(), "w");
        char line[120] = "";
        sprintf(line, "%d|%d|%d|%s\n", c->isdir, c->blockn , c->size , (c->file_name).c_str());
        fwrite(line, 120, 1, fp);
	cout << "returned from write metaf" <<endl;
        fclose(fp);
}

inline bool exists_test(const std::string& name) 
{
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

static int master_getattr(const char *pathh, struct stat *stbuf) 
{
	if (strcmp(pathh, "/") == 0) 
	{
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		cout << "getaatr success" <<endl;
		return 0;
	}
	//check if the file name exists 
	string lath(pathh);
	string p = replace (lath) ;
	string path = cwd + "/meta/"+ p ;	
	cout << "entered getattr yayy! and the path is" << path <<endl;
	if(exists_test(path))
	{
		fmeta f1;
		get_metaf(lath,&f1);
		if (f1.isdir == 1)
		{
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
			stbuf->st_size = f1.size;
		}
		else
		{
			stbuf->st_mode = S_IFREG | 0755;
			stbuf->st_nlink = 1;
		  	stbuf->st_size = f1.size;
		}
		
		return 0;
	}
	else 
	{
		return -ENOENT;

	}

}

int master_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	//string lath(path);
	//string p = replace (lath); 
	//string str = cwd + "/meta/"+ p ;
	cout << "!!!!!!!!1entered the create "<< endl;
	std::string str(path);
	//cout <<
	size_t pos = str.find_last_of('/');
	string strPath = str.substr(0, pos);
	string strName = str.substr(pos + 1);
	fmeta f1;
	f1.isdir=0;
	f1.blockn = 0;
	f1.size = 0;
	f1.file_name = strName;
	//fmeta f2;
	write_metaf(str, &f1) ;
	cout << "returned from create" <<endl;
	return 0;
}


string compute_sha(const char * buf)
{
	unsigned const char *str;
	str = (unsigned const char*)buf;
	unsigned char hash[SHA_DIGEST_LENGTH];
	char con[SHA_DIGEST_LENGTH][2];
	SHA1(str, sizeof(str), hash);
	int i;
	string fihash;
	
	for (i = 0; i < 20; i++) {

		//printf("%02x ",hash[i]);
		sprintf(con[i],"%x", hash[i] );
		//printf("%s \n",con[i]);
		fihash += con[i];
    	}
    	//printf("\n");
	//cout << "yoo fihash is " << fihash <<endl;
	std::string ret( hash, hash + sizeof hash / sizeof hash[0] );
	
	return fihash;
}


/*string compute_sha(const char * buf)
{
	unsigned const char *str;
	str = (unsigned const char*)buf;
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(str, sizeof(str), hash);
	std::string ret( hash, hash + sizeof hash / sizeof hash[0] );
	
	return ret;
} */

string getblockpath(string ff)
{
	string ret = cwd + "/block/" + ff ;
	return ret;
		
}
/*void get_inorder(string path,std::vector<std::string>& insertOrder )
{
	string file = cwd + path + ".vec";
	std::string line;
        std::ifstream myfile(file);
	std::vector<std::string> b;
	while (std::getline(myfile, line))
	{
		b.push_back(line);
	}
	myfile.close();
	for (int i = 0; i < b.size(); ++i)
        {

            const std::string &s = b[i];
            std::cout << s << "and the size is " << endl;

        }

}

void write_inorder(string path,std::vector<std::string>& insertOrder)
{	
	string file = cwd + path + ".vec";
	std::ofstream output_file(file);
	std::ostream_iterator<std::string> output_iterator(output_file, "\n");
	std::copy(insertOrder.begin(), insertOrder.end(), output_iterator);
	output_file.close();
	return;
}*/


string getblockdpath(string ff)
{
	string ret = cwd + "/data/" + ff + ".data";
	return ret;
}

int master_write(const char *pathh, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{
	cout<<"entered write"<<endl;
	string path(pathh);
	fmeta f1;
	get_metaf(path,&f1);
	string buff(buf);
/////////////////////////////////////////////////////////////////////////	
	string hash;	
	//change variables: buf ,hash, ash1; 
	//////////////////////////////////////////////////////////	
	//str = (unsigned const char*)brr;
	unsigned char ash[SHA_DIGEST_LENGTH];
	char con[SHA_DIGEST_LENGTH][2];
	SHA1((const unsigned char*)buff.c_str(), buff.size(), ash);
	std::string bash( ash, ash + sizeof ash / sizeof ash[0] );
	int i;
	for (i = 0; i < 20; i++) {
        	//printf("%02x ", hash[i]);
		sprintf(con[i],"%x", bash[i] );
		hash += con[i];
    	}	
	//////////////////////////////////////////////////////////////
	
	
	
////////////////////////////////////////////////////////////////////////////	
	
	cout <<"In the write the hash value is "<<hash <<endl;
	//cout <<"In the write the data is" << buf << endl ;
	cout << "next line after the write" << endl;	
	string blockp = getblockpath(hash);  
	cout << "in the write blockp  path is  "<< blockp <<endl;	
	string blockd;
	if(exists_test(blockp))
	{
		blockmeta c;
		get_metab(blockp,&c);
		c.ref = c.ref + 1;
		write_metab(blockp,&c);
		
	}
	else
	{
		blockmeta c;
		c.size = size;
		c.ncopy = 1;
		c.ref = 1;
		c.host1 = hash;
		write_metab(blockp,&c);
		blockd = getblockdpath(hash);
		cout << "in the write blockd  path is  "<< blockp <<endl;	
		FILE *clp;
		clp = fopen(blockd.c_str(), "wb");
		fwrite(buf, size, 1, clp);
		fclose(clp);
	}
	f1.size += size ;
	f1.blockn += 1 ;
	
	write_metaf(path, &f1);
	/////////updating the vector for the hashes;
	std::vector<std::string> insertOrder;
	string vecf = cwd +"/vector/"+ replace(path) + ".vec";
	//////////////////// Code to read vector from a file //////////////////////////
	/////////////////////////////////////////////////////
	std::string line;
	std::ifstream myfile(vecf.c_str());
	//std::vector<std::string> b;

	while (std::getline(myfile, line))
	{

		insertOrder.push_back(line);
	}
	myfile.close();
	////////////////////////////////////////////////////
	
	insertOrder.push_back(hash);
	
	///////   Code to write vector to a file //////////////////////
	////////////////////////////////////////////////////////////////////////////////
	std::ofstream output_file(vecf.c_str());
	std::ostream_iterator<std::string> output_iterator(output_file, "\n");
	std::copy(insertOrder.begin(), insertOrder.end(), output_iterator);
        output_file.close();
	///////////////////////////////////////////////////////////////////////
	

	
	
	return size ;
	
	
	

}

string get_data(string path, int s, int size)
{
	cout <<"entered get _data start is" << s <<"size is " << size <<endl ;
	FILE *temp;	
	int end = s + size ;
	int size_to_read;
	fmeta f;
	get_metaf(path,&f);
	std::vector<std::string> insertOrder;
	string vecf = cwd +"/vector/" + replace(path) + ".vec";
	string blockp,blockd;
	blockmeta c;
	string out;
	//////////////////// Code to read vector from a file //////////////////////////
	/////////////////////////////////////////////////////
	std::string line;
	std::ifstream myfile(vecf.c_str());
	//std::vector<std::string> b;

	while (std::getline(myfile, line))
	{

		insertOrder.push_back(line);
	}
	myfile.close();
	////////////////////////////////////////////////////
	int starti= 0;
	int endi =0;
	int endl =0;
	int stn =size;
	string block_str;
	char buck[10000] = {'\0'};
	for (int i = 0; i < insertOrder.size(); ++i)
	{
	    const std::string &ss = insertOrder[i];
	    //std::cout << s << "and the size is " << endl;
		//cout << "size of each vector "<< sizeof(example[i])<<endl;
		cout<< "the hash name is " << ss <<endl ;
		blockp = getblockpath(ss);
	   	get_metab(blockp,&c);
		size_to_read = c.size ;
		cout<< "the size to read is  name is " << size_to_read <<endl ;
		blockd = getblockdpath(ss);
		temp = fopen(blockd.c_str(), "rb");
		fread(buck, size_to_read, 1, temp);
		fclose(temp);
		block_str = string(buck);
		endi= starti + c.size ;
		stn -= c.size; 
		if( starti < s && s < endi)
		{
			block_str = block_str.substr(s-starti);
			stn += (s-starti);
			if(starti < end && end < endi)
			{
				block_str = block_str.substr(0,end-s);
				stn += (endl-end);
				endl=1;
			}
		}
		else if(starti < end && end < endi)
		{
			stn += (endl-end);
			block_str = block_str.substr(0,end-starti);
			endl=1;
		}
		
		out += block_str;
		starti = endi ;
		
		if (endl)
			break;
		if (stn ==0)
			break;
	}	
	return out;
}



static int master_read(const char *pathh, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	string path(pathh);
	string data;
	unsigned int len;
	cout<< "Kairav :entered read the path is"<<path << "and the size is "<<size  << endl ;
	cout<< "Kairav :entered read offset is  "<< offset <<endl ;
	 fmeta f;
	get_metaf(path,&f);
	cout << "the size of the file is " << f.size << endl ;
	len = f.size ;
	cout<< "Kairav :entered read the length is " << len <<endl ;
	if (offset > len)
	{
		cout<< "Kairav :entered read case 1 " <<endl ;
		data="";
		return 0;
	}
	else if((offset+size)> len)
	{
		cout<< "Kairav :entered read case 2 " <<endl ;
		data = get_data(path,offset, len-offset);
		memcpy(buf, data.c_str(), len - offset);
		return (len  - offset) ;
	}
	else 
	{
		cout<< "Kairav :entered read case 1 " <<endl ;
		data = get_data(path,offset, size);
		memcpy(buf,data.c_str(),size);
		return size;
	}

	
}

static int master_open(const char *pathh, struct fuse_file_info *fi)
{
	string path(pathh);
	string file = 	cwd + "/meta/"+ replace(path) ;
	if(exists_test(file))
	{
		return 0;
	}
	else
	{
		return -ENOENT;
	}
}

static int master_mkdir(const char* path, mode_t m) 
{
	std::string str(path);
	size_t pos = str.find_last_of('/');
	string strPath = str.substr(0, pos);
	string strName = str.substr(pos + 1);
	fmeta f1;
	f1.isdir=1;
	f1.blockn = 0;
	f1.size = 0;
	f1.file_name = strName;
	//fmeta f2;
	write_metaf(str, &f1) ;
	return 0;
}

static int master_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
		
	string str1(path);
	cout << "inside the readdir the path is " << str1 << endl;	
	string str = replace(str1);
	//fmeta f;
	//get_metaf(str1,&f);
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	DIR *dir;
	std::string pat;
	std::map<string, int> globalHash;
	std::map<string, int>::iterator iter;
	int r = 0;
	string wd = cwd+"/meta/";
	struct dirent *ent;
	//cout << "Kairav readdir: trying to open the file " << wd<<endl; 
	if ((dir = opendir(wd.c_str())) != NULL) 
	{
		while ((ent = readdir(dir)) != NULL)
		{
			pat = string(ent->d_name);
			std::string result;
			std::size_t found = pat.find(str);
			if(found!=std::string::npos)
			{
				found = found+str.size();
				pat.erase(0,found);
   			//std::cout << pat << std::endl;
   				std::string delimiter = "_";
   				std::string pattern = pat.substr(0, pat.find(delimiter));
				std::cout << pattern << std::endl;
				iter =  globalHash.find(pattern);
				if(iter == globalHash.end() )
				{
					globalHash.insert(std::pair<string, int>(pattern,1));
					filler(buf, pattern.c_str(), NULL, 0);
				}
				
			}
		}
	}
	else
	{
		cout <<"readdir error " <<endl;
		return -ENOENT;
	}
	return r;
}
void deletef(string str)
{
	std::vector<std::string> insertOrder;
	string vecf = cwd +"/vector/" + str + ".vec";
	blockmeta c;
	//////////////////// Code to read vector from a file //////////////////////////
	/////////////////////////////////////////////////////
	std::string line;
	std::ifstream myfile(vecf.c_str());
	string blockp;
	//std::vector<std::string> b;

	while (std::getline(myfile, line))
	{

		insertOrder.push_back(line);
	}
	myfile.close();
	////////////////////////////////////////////////////
	for (int i = 0; i < insertOrder.size(); ++i)
	{
		const std::string &ss = insertOrder[i];
		blockp = getblockpath(ss);
	   	get_metab(blockp,&c);
		c.ref -= 1;
		cout<< "In the delete file reference count of the block is " << c.ref << endl ;
		if(c.ref > 0)
		{
			write_metab(blockp,&c);
		}
		else
		{
			//code to delete the block;
		}
	}
	return ;

}
static int my_rmdir(const char* pathh) 
{
	cout << "in the rmdir call " << endl;
	string str1(pathh);
	string str = replace(str1);
	fmeta f;
	get_metaf(str1,&f);
	std::string pat;
	if(f.isdir == 0)
		return -ENOTDIR;
	string wd = cwd+"/meta/";
	struct dirent *ent;
	DIR *dir;
	while ((ent = readdir(dir)) != NULL)
	{
		pat = string(ent->d_name);
		std::string result;
		std::size_t found = pat.find(str);
		if(found!=std::string::npos)
		{
			//file found delete the file 
			deletef(str);
		}
	}
	return 0;
}

static int my_unlink(const char* path) {

	return 0;
}
/*static struct fuse_operations master_oper = {
	.getattr	= master_getattr,
	.create		= master_create,
	.write		= master_write,
	//.open		= master_open,
	.read		= master_read,
	.open		= master_open,
	.mkdir 		= master_mkdir,
};*/

int main(int argc, char *argv[]) 
{
	getcwd(cw, 120);
	cwd= string(cw);	
	static struct fuse_operations master_oper;
	master_oper.getattr = master_getattr;
	master_oper.open = master_open;
	master_oper.readdir = master_readdir;
	master_oper.mkdir = master_mkdir;
	master_oper.rmdir = my_rmdir;
	master_oper.create = master_create;
	master_oper.read = master_read;
	master_oper.write = master_write;
	//master_oper.rename = my_rename;
	//master_oper.unlink = my_unlink;
	//master_oper.truncate = my_truncate;
	//master_oper.utimens = my_utimens;
	/* 		Code to start socket	*/

	int sock_id,new_socket;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	// Main Socket which will be listening for connections.
	//Listening Socket creation
	sock_id = socket (AF_INET, SOCK_STREAM, 0);
	
	if (sock_id == -1)
	{
		printf("\nError: Opening socket\n");
		perror("");
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	//Socket address assignment
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(7734);
	

	//Bind socket with port
	if (bind(sock_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
	{
		printf("\nError: Binding not successful\n");
		perror("");		
		return 1;
	}
	printf("Bind successful\n");

	//Listening to connections
	if(listen(sock_id, 5) == -1)
	{
		printf("\nFailed to listen\n");
		return 1;
	}
	printf("Listening to port 7734\n"); // this line exec whenever a new client tries to communicate.

	//Accepting connections from all the clients.
	clilen = sizeof(cli_addr);

	int random =1;	
	
	for(int kk=0;kk<1;kk++)
	{
		
		new_socket = accept(sock_id, (struct sockaddr *) &cli_addr, &clilen);
		//printf("\n%s",str2);	

		if (new_socket<0)
		{
			perror ("accept failed");
			return 1;
		}

		//Thread creation
		pthread_t p1;

		if (pthread_create(&p1, NULL, probe_connection_handler, (void*) &new_socket) < 0)
		{
			perror("Error: Could not create thread");
			return 1;
		}
		printf ("\nHandler assigned");	
		
		
	
			
		
	}
	int k=99;
	sleep(20);
	if (write(socket_list[0],(char *)&k,sizeof(k)) < 0)
	{
		printf("Error: Send failed");
		return 0;
	}	
	
	/******************/
	
	fuse_main(argc, argv, &master_oper, NULL);
	
}
