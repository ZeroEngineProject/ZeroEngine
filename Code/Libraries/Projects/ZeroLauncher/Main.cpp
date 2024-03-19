// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

using namespace Zero;

extern "C" int main(int argc, char* argv[])
{
  CommandLineToStringArray(gCommandLineArguments, argv, argc);
  SetupApplication(1, sZeroOrganization, sLauncherGuid, sLauncherName);

  LauncherStartup* startup = new LauncherStartup();
  int returnCode = startup->Run();

  return returnCode;
}
