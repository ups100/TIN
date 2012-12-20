///////////////////////////////////////////////////////////
//  AliasFileList.h
//  Implementation of the Class AliasFileList
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_)
#define EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_

#include <QByteArray>

namespace TIN_project {
namespace Utilities {

class AliasFileList
{

public:
    AliasFileList();
    AliasFileList(const QByteArray &bytes);
    virtual ~AliasFileList();

    QByteArray toQByteArray();
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_)
