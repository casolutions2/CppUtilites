#ifndef casDateTimeH
#define casDateTimeH

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <format>
#include <random>
#include <array>
#include <iterator>

using namespace std::chrono;

// to add a specific duration to a timepoint need to provide a typedef 
// typedef std::chrono::time_point< std::chrono::system_clock > timepoint_t;
typedef std::chrono::duration<double> double_prec;
typedef std::chrono::time_point< std::chrono::system_clock, double_prec > timepoint_t;

using ten_min = std::chrono::duration<int, std::ratio<600>>;
using fifteen_min = std::chrono::duration<int, std::ratio<60*15>>;
using thirty_min = std::chrono::duration<int, std::ratio<60*30>>;
using one_hr = std::chrono::duration<int, std::ratio<60*60>>;


template<typename Period>
auto Ceil2( auto tp ) {
    return std::chrono::ceil<std::chrono::duration<int,Period>>(tp);
}

template<typename D>
auto Ceil3(const auto tp, const int m=0) {
    // auto mins = std::chrono::minutes{m};
    return std::chrono::ceil<D>(tp);
}

template<typename Duration>
class UsesDuration {
    public:
        auto operator()(auto tp) { 
            return ceil<decltype(my_duration)>(tp); 
        }
    private:
        Duration my_duration;
};

namespace casq {
    class datestamp {
        public:
            datestamp() {};
            explicit datestamp( std::string ds, std::string fmt);
            auto get_date() {
                return time_point_;
            }
            friend bool operator < (const datestamp& lhs, const datestamp& rhs) {
                return lhs.time_point_ < rhs.time_point_;
            }
            auto AddDays( int d) {
                time_point_ += std::chrono::days(d);
            }
            friend std::ostream& operator<<( std::ostream& os, const datestamp& ds ) {
                return os << ds.time_point_;
            }

        private:
            std::chrono::time_point<std::chrono::system_clock, days> time_point_;

    }; // end class datestamp
    class timestamp {
        public:
            timestamp() {};
            explicit timestamp(const std::string ts, const std::string fmt);
            auto get_time() {
                return time_point_;
            }
            int get_period() {
                return time_point_.count();
            }
            friend bool operator<(const timestamp& lhs, const timestamp& rhs) {
                return lhs.time_point_ < rhs.time_point_;
            }
            friend std::ostream& operator<<(std::ostream& os, const timestamp& ts) {
                return os << ts.time_point_;
            }

        private:
            thirty_min time_point_;

    }; // end class timestamp

    datestamp::datestamp( std::string ds, std::string fmt) {
        std::stringstream ss; ss << ds;
        ss >> std::chrono::parse(fmt, time_point_);
        if (!ss.fail()) 
            return;
        else
            throw std::runtime_error("parsing failed");
    }
    timestamp::timestamp( std::string ts, std::string fmt) {
        std::stringstream ss; ss << ts;
        ss >> std::chrono::parse(fmt, time_point_);
    }
}; // end namespace casq

// ===========================================================================================
void check_class() {
    casq::datestamp ds("4/12/24", "%d/%m/%y");
    std::stringstream xs("25/12/24");
    casq::datestamp xd("25/12/24", "%d/%m/%y");
    std::cout << "casq::datestamp is " << ds << '\n';
    ds.AddDays(2);
    std::cout << "casq::datestamp is " << ds << '\n';
    if ( ds < xd )
        std::cout << ds << " is less than " << xd << '\n';
    else
        std::cout << ds << " is greater than or equal to " << xd << '\n';
    
    casq::timestamp ts("11:00:00", "%H:%M:%S");
    std::cout << '\n' << ts << '\n';
    std::cout << "Period: " << ts.get_period() << '\n';
}
void check_parse() {
    std::cout << "check_parse\n";
    std::chrono::time_point<std::chrono::system_clock, minutes> tp;
    
    std::stringstream ss {"4/12/24"}; std::stringstream ts{"1/12/24 17:30"};
    std::cout << "stringstream : " << ss.str() << ", " << ts.str() << '\n';
    //ss >> std::chrono::parse("%d/%m/%y",dp);
    ts >> std::chrono::parse("%d/%m/%y %H:%M",tp);
    std::cout << "results: " << /*dp << ", " <<*/ tp << '\n';
    
    // use seconds 
    std::stringstream sstime{"00:00:00"};
    thirty_min tp_time;
    sstime >> std::chrono::parse("%H:%M:%S", tp_time);
    int period = tp_time.count() / 30;
    std::cout << "minutes: " << tp_time << " which is period: " << period << '\n';
}


int main(int argc, char** argv) {
    std::cout << std::format("starting {} >>>\n\n", argv[0]) ;
    // check_parse();
    check_class();
    // return 0;
    try {
        auto tz = std::chrono::locate_zone("Australia/Brisbane");
        std::cout << "Timezone: " << tz->name() << '\n';

        auto utc_tp = std::chrono::system_clock::now();
        std::cout << "utc tp: " << utc_tp << '\n';
        auto local_tp = tz->to_local(utc_tp);
        std::cout << "local tp: " << local_tp << '\n';
        auto tp = local_tp;
        typedef ten_min duration;        
                typedef fifteen_min duration;        
        UsesDuration<duration> x; 
        std::cout << "x: " << x(tp) << '\n';
        
    } catch ( std::exception& ex ) {
        std::cout << ex.what() << '\n';
    }
    std::cout << std::format("\n{} completed!", argv[0]) << '\n';
    return 0;
}

#endif
