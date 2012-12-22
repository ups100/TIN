#if !defined(INTERPROCESS_NAME__INCLUDED_)
#define INTERPROCESS_NAME__INCLUDED_

namespace TIN_project {
namespace Utilities {

/**
 * Contains interprocess socket name for Client - Daemon communication
 */
class InterprocessName
{
public:
    /** Name for Unix socket to client-daemon communication */
    static const char *INTERPROCESS_NAME;
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(INTERPROCESS_NAME__INCLUDED_)
