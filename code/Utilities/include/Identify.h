#if !defined(IDENTIFY__INCLUDED_)
#define IDENTIFY__INCLUDED_

#include <QString>

namespace TIN_project {
namespace Utilities {

/**
 * Class responsible for getting identification data of actual machine
 *
 * @author Kajo0
 */
class Identify
{
    /**
     * @breif Private C-tor
     */
    Identify();

    /**
     * @breif Private Copying C-tor
     */
    Identify(const Identify &other);

    /**
     * @brief Private Assignment operator
     */
    Identify& operator=(const Identify &other);

public:

    /*
     * @brief D-tor
     */
    virtual ~Identify();

    /**
     * @brief Get MAC address
     * @return Only the first non-loopback MAC Address
     */
    static QString getMachineIdentificator();

};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(IDENTIFY__INCLUDED_)
