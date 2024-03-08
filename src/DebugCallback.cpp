#include "oura_charts/RestDataProvider.h" 

namespace oura_charts::detail
{

   
   void LoggingCallback::operator()(cpr::DebugCallback::InfoType type, std::string data, intptr_t userdata)
   {
      assert(m_logger.get());

      using cpr::DebugCallback;
      using std::string;

      string msgtype{ "Unknown" };
      switch (type)
      {
      case DebugCallback::InfoType::TEXT:
         msgtype = "Text";
         break;
      case DebugCallback::InfoType::HEADER_IN:
         msgtype = "Header In";
         break;
      case DebugCallback::InfoType::HEADER_OUT:
         msgtype = "Header Out";
         break;
      case DebugCallback::InfoType::DATA_IN:
         msgtype = "Data In";
         break;
      case DebugCallback::InfoType::DATA_OUT:
         msgtype = "Data Out";
         break;
      case DebugCallback::InfoType::SSL_DATA_IN:
         msgtype = "SSL Data Out";
         break;
      case DebugCallback::InfoType::SSL_DATA_OUT:
         msgtype = "SSL Data Out";
         break;
      default:
         std::unreachable();
      };
      m_logger->debug("REST Provider Debug Trace ({}) - {}", msgtype, data);
   }

}// namespace oura_charts::detail
