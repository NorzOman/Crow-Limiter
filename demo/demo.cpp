

#include <crow.h>
// -- path might change depending on where its installed.
#include "../crow-limiter/limiter.h"

int main(){
    // -- put the middleware name limiter --
	crow::App<limiter> app;

	// -- define the limter for each route --
	// format limiter::limit(<path-in-quotes>,rate-allowed,time-windw)
	// the below line translates to allow 2 hits on the routes / for each 10 second per IP
	limiter::limit("/",2,10);
	CROW_ROUTE(app,"/")([](){
		return crow::response(200,"Hello World");
	});

	// -- another limiter for another route --
	limiter::limit("/test",2,10000);
	CROW_ROUTE(app,"/test")([](){
		return crow::response(200,"Test");
	});

	// -- not necesssary but can be helpful during testing --
	// -- this prints debug in the console , should be disabled in production  --
	limiter::limit("/debug",100,10000);
	CROW_ROUTE(app,"/debug")([](){
	    limiter::debug();
		return crow::response(200,"Debug has been printed on teh console");
	});

	// -- can you multithreading , mutex might bottleneck , but still work --
	app.port(5000).multithreaded().run();
}
