#include "http_server.hh"
#include "html_parser.hh"

#include <vector>

#include <sys/stat.h>
#include <ctime>
#include <fstream>
#include <sstream>

string readFileIntoString(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

HTTP_Request::HTTP_Request(string request) {
  vector<string> lines = split(request, '\n');
  vector<string> first_line = split(lines[0], ' ');

  this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request
  this->method = first_line[0];

  if (this->method != "GET") {
    cerr << "Method '" << this->method << "' not supported" << endl;
    exit(1);
  }

  this->url = first_line[1];
}

HTTP_Response *handle_request(string req) {
  HTTP_Request *request = new HTTP_Request(req);

  HTTP_Response *response = new HTTP_Response();

  string url = string("html_files") + request->url;

  response->HTTP_version = "1.0";

  struct stat sb;
  if (stat(url.c_str(), &sb) == 0) // requested path exists
  {
    response->status_code = "200";
    response->status_text = "OK";
    response->content_type = "text/html";
    cout << url << endl;

    string body;

    if (S_ISDIR(sb.st_mode)) {
      /*
      In this case, requested path is a directory.
      TODO : find the index.html file in that directory (modify the url
      accordingly)
      */
      if (url.back() == '/')
        url = url + "index.html";
      else
        url = url + "/index.html";
    }

    response->body = readFileIntoString(url);
    response->content_length = to_string((response->body).length());
  }
  else if( url.find('?') != string::npos ){
    response->status_code = "200";
    response->status_text = "OK";
    response->content_type = "text/html";
    vector<string> words = split(request->url, '?');
    string body;
    body = "<!DOCTYPE html>\n";
    cout << words[0] << url << endl;
    if (words[0] == "/hello" or words[0] == "hello"){
      body = body + "<html lang=\"en\">\n<body> \n<h2> Hello {{name}} !</h2>\n</body>\n</html>"; 
      body = nameparser(body, words[1]);
    }
    else if(words[0] == "/fib" or words[0] == "fib"){
      body = body + "<html lang=\"en\"> <h2> The {{n}} fibonacci number is {{fib}} </h2> </html>";
      body = fibparser(body, stoi(words[1]));
    }
    
    response->body = body;
    response->content_length = to_string((response->body).length());
  }

  else {
    response->status_code = "404";
    response->status_text = "Not Found";
  }

  delete request;

  return response;
}

string HTTP_Response::get_string() {
  time_t now = time(0);
  char* dt = ctime(&now);
  string res;
  res = "HTTP/" + this->HTTP_version + " " + this->status_code + " " + this->status_text +"\nDate: ";
  res.append(dt);
  res = res + "Content-Type: " + this->content_type + " \n";
  res = res + "Content-Length: " + this->content_length + "\n\n";
  res = res + this->body;

  return res;
}
