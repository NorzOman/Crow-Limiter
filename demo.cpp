
#include <crow.h>
#include "crow-limiter/limiter.h"

int main(){
	crow::App<limiter> app;

	limiter::limit("/",2,10);
	CROW_ROUTE(app,"/")([](){
		return crow::response(200,"Hello World");
	});

	limiter::limit("/test",5,10);
	CROW_ROUTE(app,"/test")([](){
		return crow::response(200,"Test");
	});

	app.port(5000).multithreaded().run();
}
