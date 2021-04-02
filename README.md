# raxlog
It's a simple code to loging

example of use:

int main()
{
  rsx::log::init(); //it's necesry
  
  Log("Hello World", rxs::log::INFO); // or L_INFO("Hello World");
  Log(123, rxs::log::WARN); // or L_WARN(123);
  Log(false, rxs::log::ERROR) // or L_ERROR(false);
  
  rxs::log::deinit(); //It's not necesery
}
