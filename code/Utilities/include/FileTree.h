/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include <QtCore/QFileInfo>
 #include <QtCore/QDir>
 #include <QtCore/QVector>
 #include <QtXmlPatterns/QSimpleXmlNodeModel>

namespace TIN_project {
namespace Utilities {

 class FileTree : public QSimpleXmlNodeModel
 {
  public:
     FileTree(const QXmlNamePool &namePool);

     QXmlNodeModelIndex nodeFor(const QString &fileName) const;

     virtual QXmlNodeModelIndex::DocumentOrder compareOrder(const QXmlNodeModelIndex&, const QXmlNodeModelIndex&) const;
     virtual QXmlName name(const QXmlNodeModelIndex &node) const;
     virtual QUrl documentUri(const QXmlNodeModelIndex &node) const;
     virtual QXmlNodeModelIndex::NodeKind kind(const QXmlNodeModelIndex &node) const;
     virtual QXmlNodeModelIndex root(const QXmlNodeModelIndex &node) const;
     virtual QVariant typedValue(const QXmlNodeModelIndex &node) const;
  protected:
     virtual QVector<QXmlNodeModelIndex> attributes(const QXmlNodeModelIndex &element) const;
     virtual QXmlNodeModelIndex nextFromSimpleAxis(SimpleAxis, const QXmlNodeModelIndex&) const;

  private:
     enum Type {
         File,
         Directory,
         AttributeFileName,
         AttributeFilePath,
         AttributeSize,
         ModificationDate,
         AttributeMIMEType,
         AttributeSuffix
     };

     inline QXmlNodeModelIndex nextSibling(const QXmlNodeModelIndex &nodeIndex,
                                           const QFileInfo &from,
                                           qint8 offset) const;
     inline const QFileInfo &toFileInfo(const QXmlNodeModelIndex &index) const;
     inline QXmlNodeModelIndex toNodeIndex(const QFileInfo &index,
                                           Type attributeName) const;
     inline QXmlNodeModelIndex toNodeIndex(const QFileInfo &index) const;

     /*
      One possible improvement is to use a hash, and use the &*&value()
      trick to get a pointer, which would be stored in data() instead
      of the index.
     */
     mutable QVector<QFileInfo>  m_fileInfos;
     const QDir::Filters         m_filterAllowAll;
     const QDir::SortFlags       m_sortFlags;
     QVector<QXmlName>           m_names;
 };

}
}