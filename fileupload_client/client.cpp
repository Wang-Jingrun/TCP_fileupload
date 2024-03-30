#include <string>
using std::string;

#include <socket/client_socket.h>
using namespace yazi::socket;
using namespace yazi::utility;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: client file1 file2 file3 ..." << std::endl;
		return 1;
	}

	Singleton<Logger>::instance()->open("./client.log");
	Singleton<Logger>::instance()->set_console(false);
	ClientSocket client("127.0.0.1", 8080);

	for (int i = 1; i < argc; i++)
	{
		string file = argv[i];
		printf("%s\r\n", file.c_str());

		if (!client.send_file(file))
		{
			log_error("send file error: errno=%d errmsg=%s", errno, strerror(errno));
		}
	}

	return 0;
}