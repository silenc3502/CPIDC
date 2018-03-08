#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid_s;
  PID pid_t;
  // TODO: Initialize the pid variable.

  /* Steering Part */
  //pid.Init(3, 0.0, 2.4); Big Overshoot
  //pid.Init(2.7, 0.0, 3.3); Still Big
  //pid.Init(2.5, 0.0, 7.7); Now it's work. However big vibration.

  //pid.Init(4.5, 1.5, 8.8); It makes some slow response.
  //pid.Init(0.45, 0.15, 0.88); Still it makes slow response.
  //pid.Init(7.7, 1.5, 10.0); Still slow
  //pid.Init(15.0, 1.5, 10.0); Still slow(need to decrease integral error!)
  //pid.Init(15.0, 0.75, 10.0); Bad Integral Erro!
  //pid.Init(15.0, 0.37, 10.0); Still ...

  // This is PID Controller for steering.
  pid_s.Init(13.0, 0.02, 300.0); // Also good!
  //pid_s.Init(10.0, 0.3, 22.0);	//So many frequency element!
  //pid_s.Init(10.0, 0.3, 33.0);	//So many frequency element!
  //pid_s.Init(33.0, 0.8, 10000.0);	//So many frequency element!
  //pid_s.Init(0.033, 0.0008, 100);

  /* Speed Control Part */
  //pid_t.Init(0.5, 0.2, 0.0);	// Back moving fast
  //pid_t.Init(0.1, 0.07, 0.0);	// Back moving
  pid_t.Init(0.01, 0.007, 0.0);
  // This is PI Controller for Throttle.

  h.onMessage([&pid_s, &pid_t](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value = 0;
          //double throttle_value = 0;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          pid_s.UpdateError(cte);
          steer_value -= pid_s.TotalError();

          //pid_t.UpdateError(fabs(cte));
          //throttle_value = 1.0 - (pid_t.Kp * pid_t.p_error + pid_t.Ki * pid_t.i_error);
          
          
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.4;
          //msgJson["throttle"] = 0.5;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
