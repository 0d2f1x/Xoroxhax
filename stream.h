#include "vars.h"
#include "hex.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

void on_open(client* c, websocketpp::connection_hdl hdl) {
	c->send(hdl, "Connected!", websocketpp::frame::opcode::text);
}

void on_fail(client* c, websocketpp::connection_hdl hdl) {
	c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
	Hex::flash();
	c->send(hdl, Hex::rad(), websocketpp::frame::opcode::text);
	Sleep(var.streamDelay);
}

void on_close(client* c, websocketpp::connection_hdl hdl) {
	c->get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
}

void WebSocket()
{
	client c;

	std::string uri = "ws://xorox.herokuapp.com";
	//std::string uri = "ws://localhost";

	//if (argc == 2) uri = argv[1];
	try {
		// set logging policy if needed
		c.clear_access_channels(websocketpp::log::alevel::frame_header);
		c.clear_access_channels(websocketpp::log::alevel::frame_payload);
		//c.set_error_channels(websocketpp::log::elevel::none);

		// Initialize ASIO
		c.init_asio();

		// Register our handlers
		c.set_open_handler(bind(&on_open, &c, ::_1));
		c.set_fail_handler(bind(&on_fail, &c, ::_1));
		c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
		c.set_close_handler(bind(&on_close, &c, ::_1));

		// Create a connection to the given URI and queue it for connection once
		// the event loop starts
		websocketpp::lib::error_code ec;
		client::connection_ptr con = c.get_connection(uri, ec);
		c.connect(con);

		// Start the ASIO io_service run loop
		c.run();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (websocketpp::lib::error_code e) {
		std::cout << e.message() << std::endl;
	}
	catch (...) {
		std::cout << "other exception" << std::endl;
	}
}