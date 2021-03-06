﻿#include "stdafx.h"
#include "modio.h"

int main()
{
  // Retreive your game id and api key from the mod.io web to initialize mod.io
  // First, experiment with the test environment under test.mod.io and once you're ready to launch, switch to the live environment by passing the MODIO_ENVIRONMENT_LIVE flag
  modio::Instance modio_instance(MODIO_ENVIRONMENT_TEST, YOUR_GAME_ID, "YOUR_API_KEY");

  volatile static bool finished;
  
  auto wait = [&]() {
    finished = false;
    while (!finished)
    {
      modio_instance.sleep(10);
      // Add the process funcion to your game loop to process mod.io funcionality asynchronously
      modio_instance.process();
    }
  };

  auto finish = [&]() {
    finished = true;
  };

  modio_instance.setDownloadListener([&](u32 response_code, u32 mod_id) {
    std::cout << "Download mod response: " << response_code << std::endl;

    if (response_code == 200)
    {
      std::cout << "Mod " << mod_id << " downloaded successfully" << std::endl;
      // installDownloadedMods() is a blocking function that extract and installs the downloaded mods
      // Alternatively, downloaded mods are also extracted on mod.io initialization
      modio_instance.installDownloadedMods();
      std::cout << "Installed downloaded mods." << std::endl;
    }
  });

  // Check to see if we have a cookie and are already logged in
  if (!modio_instance.isLoggedIn())
  {
    std::string email;

    std::cout << "Enter your email: " << std::endl;
    std::cin >> email;

    // Auth works by sending an email with a code. Lets trigger that now
    modio_instance.emailRequest(email, [&](const modio::Response &response) {
      std::cout << "Response code: " << response.code << std::endl;

      if (response.code == 200)
      {
        std::string securityCode;
        std::cout << "Please enter the 5 digit security code: ";
        std::cin >> securityCode;

        // Finish the auth process by entering the security code
        modio_instance.emailExchange(securityCode, [&](const modio::Response &response) {
          std::cout << "Response code: " << response.code << std::endl;
          if (response.code == 200)
          {
            std::cout << "Code exchanged!" << std::endl;
          }
          else
          {
            std::cout << "Error exchanging code" << std::endl;
          }
          finish();
        });
      }
      else
      {
        std::cout << "Error sending code" << std::endl;
      }
    });
    wait();
  }else
  {
    std::cout << "You are already logged in" << std::endl;
  }

  std::cout << "Download will be triggered automatically when you subscribe" << std::endl;
  std::cout << "Waiting for mod downloads..." << std::endl;

  wait();

  return 0;
}
