#include <iostream>
#include <time.h>

time_t get_timestamp(const char *date = NULL,
                     const char *format = "%Y-%m-%d %H:%M:%S") {
  struct tm tm = {0};
  if (!date) {
    return time(NULL);
    // return absl::ToUnixMillis(absl::Now());
  }
  strptime(date, format, &tm);
  return mktime(&tm);
}

int main(void) {
  char date[] = "2018-02-10 19:00:00";
  time_t now = get_timestamp(date);
  std::cout << "date " << date << " to timestamp " << now << std::endl;
  
  now = get_timestamp();
  std::cout << "now timestamp " << now << std::endl;

  return 0;
}
