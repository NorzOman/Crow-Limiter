## Crow Rate Limiter

A simple, **header-only module** to implement rate limiting for each path, exclusive to each client. This is well suited for **testing and hobbyist projects**, but **not recommended for production-based systems**.

#### How Do I Use This?
1.  Go to the **releases** section of the repository.
2.  The `*.zip` file contains the header files and related information.
3.  Integrate it into your project. [You may refer to the `demo/` provided in this repository to understand the integration process.]

#### How Do I Integrate This?
1.  After placing the header file, include it in your project:
    `#include "crow-limiter/limiter.h"`
2.  Then, set the application to use the provided middleware:
    `crow::App<limiter, other middlewares..> app;`
3.  Next, set the limit for each route:
    `limiter::limit("/", 5, 30)`
    This setting allows **5 requests per 30 seconds**.
4.  For compilation, use `-I./crow-limiter` to include the header.

#### Why Use This?
This will effectively **restrict usage for users**, preventing them from flooding any resources. However, it is important to note that this throttling is performed only on the **application layer**, **not the network layer**. Therefore, it will **not directly protect against various types of DDoS attacks**.

#### Footnotes
I plan to add more features, perhaps a dashboard to check analytics, or maybe create another project based on this. If you have good features to add, feel free to **fork and contribute to this repo**.
