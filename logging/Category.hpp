#ifndef	CATEGORY_HPP
#define	CATEGORY_HPP 1

#include <log4cpp/Category.hh>
#include "LoggingEvent.hpp"
#include <rtt/Port.hpp>

namespace OCL {
namespace logging {

// forward declare
class LoggingService;

/** A real-time capable category
    \warning Private inheritance to hide all func's with std::string in
    base class.
*/
class Category : public log4cpp::Category
{
public:
    virtual ~Category();

    // custom real-time versions - available to user
    // these replace std::string versions in the base class
public:
    virtual void log(log4cpp::Priority::Value priority, 
                     const RTT::rt_string& message) throw();
    void debug(const RTT::rt_string& message) throw();
    void info(const RTT::rt_string& message) throw();
    void notice(const RTT::rt_string& message) throw();
    void warn(const RTT::rt_string& message) throw();
    void error(const RTT::rt_string& message) throw();
    void crit(const RTT::rt_string& message) throw();
    void alert(const RTT::rt_string& message) throw();
    void emerg(const RTT::rt_string& message) throw();
    void fatal(const RTT::rt_string& message) throw();

protected:
    void _logUnconditionally2(log4cpp::Priority::Value priority, 
                              const RTT::rt_string& message) throw();


    // real-time - available to user
public:
    using log4cpp::Category::setPriority;
    using log4cpp::Category::getPriority;
    using log4cpp::Category::getChainedPriority;
    using log4cpp::Category::isPriorityEnabled;

    using log4cpp::Category::setAdditivity;
    using log4cpp::Category::getAdditivity;
    using log4cpp::Category::getParent;

    using log4cpp::Category::isDebugEnabled;
    using log4cpp::Category::isInfoEnabled;
    using log4cpp::Category::isNoticeEnabled;
    using log4cpp::Category::isWarnEnabled;
    using log4cpp::Category::isErrorEnabled;
    using log4cpp::Category::isCritEnabled;
    using log4cpp::Category::isAlertEnabled;
    using log4cpp::Category::isEmergEnabled;
    using log4cpp::Category::isFatalEnabled;


    // real-time (but uses locking) - available to user but BEWARE locking!
public:
    using log4cpp::Category::getAppender;



    // NOT real-time and so _NOT_ available to user
protected:
    using log4cpp::Category::getRoot;
    using log4cpp::Category::setRootPriority;
    using log4cpp::Category::getRootPriority;
    using log4cpp::Category::getInstance;
    using log4cpp::Category::exists;
    using log4cpp::Category::getCurrentCategories;
    using log4cpp::Category::shutdown;
    using log4cpp::Category::getName; 

    using log4cpp::Category::removeAllAppenders;
    using log4cpp::Category::removeAppender;
    using log4cpp::Category::addAppender;
    using log4cpp::Category::setAppender;

    using log4cpp::Category::getAllAppenders;

    using log4cpp::Category::debugStream;
    using log4cpp::Category::infoStream;
    using log4cpp::Category::noticeStream;
    using log4cpp::Category::warnStream;
    using log4cpp::Category::errorStream;
    using log4cpp::Category::critStream;
    using log4cpp::Category::emergStream;
    using log4cpp::Category::fatalStream;
    using log4cpp::Category::getStream;
    using log4cpp::Category::operator<<;

    using log4cpp::Category::callAppenders;

    using log4cpp::Category::log;
    using log4cpp::Category::logva;
    using log4cpp::Category::debug;
    using log4cpp::Category::info;
    using log4cpp::Category::notice;
    using log4cpp::Category::warn;
    using log4cpp::Category::error;
    using log4cpp::Category::crit;
    using log4cpp::Category::alert;
    using log4cpp::Category::emerg;
    using log4cpp::Category::fatal;

    using log4cpp::Category::_logUnconditionally;
    using log4cpp::Category::_logUnconditionally2;
        

protected:
	Category(const std::string& name,   
             log4cpp::Category* parent, 
             log4cpp::Priority::Value priority = log4cpp::Priority::NOTSET);

protected:
    /** Send \a event to all attached appenders.
        \param event The event of interest
        \note Real-time capable for all attached OCL-configured appenders
    */
    virtual void callAppenders(const OCL::logging::LoggingEvent& event) throw();

    /** Convert \a name into Orocos notation (e.g. "org.me.app" -> "org_me_app")

        \warning Not real-time capable
    */
    static std::string convertName(const std::string& name);    

public:
    /** Factory function for log4cpp::HierarchyMaintainer
        Creates an OCL logging category.
        
        \warning Not real-time capable
    */
    static log4cpp::Category* createOCLCategory(const std::string& name, 
                                                log4cpp::Category* parent, 
                                                log4cpp::Priority::Value priority);

protected:
//protected:
    RTT::OutputPort<OCL::logging::LoggingEvent>   log_port;
    /// for access to \a log_port
    friend class OCL::logging::LoggingService;
    
private:
    /* prevent copying and assignment */
    Category(const Category& other);
    Category& operator=(const Category& other);
};

// namespaces
}
}

#endif
