/* -------------------------------------------------------------------------
//	FileName	：	hm_xml.h
//	Creator		：	(zc)
//	CreateTime	：	2013-04-18 10:08:26
//	Description	：	
//
// -----------------------------------------------------------------------*/
#ifndef __HM_XML_H__
#define __HM_XML_H__
#include "zccommon.h"
#include "zccommon_export.h"

// #define YG_USE_TINYXML
#ifdef YG_USE_TINYXML
	class TiXmlNode;
	class TiXmlAttribute;
	class TiXmlDocument;
#else
	class RdXmlNode;
	class RdXmlAttribute;
	class RdXmlDocument;
#endif // !YG_USE_TINYXML
// -------------------------------------------------------------------------
NS_BEGIN(base)

//////////////////////////////////////////////////////////////////////////
// HMXmlAttribute
class ZCCOMMON_EXPORT HMXmlAttribute
{
public:
	HMXmlAttribute();
	/*
	@func		: isValid
	@brief		: 是否可用。
	*/
	bool isValid() const;
	/*
	@func		: isEqual
	@brief		: 是否相同。
	*/
	bool isEqual(const HMXmlAttribute& other) const;
	/*
	@func		: Name
	@brief		: 
	*/
	LPCSTR	getName()  const;
	/*
	@func		: Value
	@brief		: 
	*/
	utf8string	getValue() const;
	/*
	@func		: Next
	@brief		: 
	*/
	HMXmlAttribute nextAttribute() const;
private:
#ifdef YG_USE_TINYXML
	TiXmlAttribute*			xmlAttr__;
#else
	RdXmlAttribute*			xmlAttr__;
#endif // !YG_USE_TINYXML
	friend class HMXmlElement;
};

//////////////////////////////////////////////////////////////////////////
// HMXmlNode
class ZCCOMMON_EXPORT HMXmlNode
{
public:
	HMXmlNode();
	/*
	@func		: isValid
	@brief		: 是否可用。
	*/
	bool isValid() const;
	/*
	@func		: isEqual
	@brief		: 是否相同。
	*/
	bool isEqual(const HMXmlNode& other) const;
	/*
	@func		: unspecified_bool_type
	@brief		: 增加bool值判定。by ZC. 2012-10-19 10:49.
	*/
#ifdef YG_USE_TINYXML
	typedef TiXmlNode* HMXmlNode::*unspecified_bool_type;
#else
	typedef RdXmlNode* HMXmlNode::*unspecified_bool_type;
#endif // !YG_USE_TINYXML
	operator unspecified_bool_type() const {
		return xmlNode__ ? &HMXmlNode::xmlNode__ : NULL;
	}
	/*
	@func		: firstChild
	@brief		:
	*/
	HMXmlNode firstChild(LPCSTR xmlTag = NULL) const;
	/*
	@func		: lastChild
	@brief		:
	*/
	HMXmlNode lastChild(LPCSTR xmlTag = NULL) const;
	/*
	@func		: previousSibling
	@brief		:
	*/
	HMXmlNode previousSibling(LPCSTR xmlTag = NULL) const;
	/*
	@func		: nextSibling
	@brief		: Navigate to a sibling node.
	*/
	HMXmlNode nextSibling(LPCSTR xmlTag = NULL) const;
protected:
#ifdef YG_USE_TINYXML
	TiXmlNode*				xmlNode__;
#else
	RdXmlNode*				xmlNode__;
#endif // !YG_USE_TINYXML
	friend class HMXmlDocHandle;
};

//////////////////////////////////////////////////////////////////////////
// HMXmlElement
class ZCCOMMON_EXPORT HMXmlElement
	: public HMXmlNode
{
public:
	/*
	@func		: tagName
	@brief		: 如果当前节点是element，则返回其标签名。
	*/
	LPCSTR tagName() const;
	/*
	@func		: isTagName
	@brief		: 是否是指定的标签名。
	*/
	bool isTagName(LPCSTR pszTag);
	/*
	@func		: getText
	@brief		: 如果当前节点含有TiXmlText节点，则返回相应的值，否则，返回NULL
				  宽字符版本则返回“”字符串
	*/
	utf8string getText() const;
	/*
	@func		: attribute
	@brief		:
	*/
	utf8string attribute(IN LPCSTR name, IN LPCSTR pszDefValue = NULL) const;
	/*
	@func		: attributeInt
	@brief		:
	*/
	int32 attributeInt(IN LPCSTR name, int32 nDefValue = 0) const;
	/*
	@func		: firstChildElement
	@brief		:
	*/
	HMXmlElement firstChildElement(LPCSTR xmlTag = NULL) const;
	/*
	@func		: nextSiblingElement
	@brief		: Navigate to a sibling node.
	*/
	HMXmlElement nextSiblingElement(LPCSTR xmlTag = NULL) const;
	/*
	@func		: firstAttribute
	@brief		: Sets an attribute of name to a given value. The attribute
				  will be created if it does not exist, or changed if it does.
	*/
	HMXmlAttribute firstAttribute() const;
};

//////////////////////////////////////////////////////////////////////////
// HMXmlDocHandle
class ZCCOMMON_EXPORT HMXmlDocHandle
{
protected:
	HMXmlDocHandle(
#ifdef YG_USE_TINYXML
	TiXmlDocument* xmlDoc
#else
	RdXmlDocument* xmlDoc
#endif // !YG_USE_TINYXML
	);
public:
	/*
	@func		: isValid
	@brief		: 是否可用。
	*/
	bool isValid() const;
	/*
	@func		: isEqual
	@brief		: 是否相同。
	*/
	bool isEqual(const HMXmlDocHandle& other) const;
	/*
	@func		: isError
	@brief		:
	*/
	bool isError() const;
	/*
	@func		: rootElement
	@brief		:
	*/
	HMXmlElement rootElement(LPCSTR xmlTag = NULL) const;
	/*
	@func		: loadFromString
	@brief		:
	*/
	bool loadFromString(IN const utf8string xmlString);
protected:
#ifdef YG_USE_TINYXML
	TiXmlDocument* xmlDoc__;
#else
	RdXmlDocument* xmlDoc__;
#endif // !YG_USE_TINYXML
	bool		   xmlError__;
};

//////////////////////////////////////////////////////////////////////////
// HMXmlDocument
class ZCCOMMON_EXPORT HMXmlDocument
	: public HMXmlDocHandle
{
public:
	HMXmlDocument();
	~HMXmlDocument();
};

//////////////////////////////////////////////////////////////////////////
// HMXmlDocument2
class ZCCOMMON_EXPORT HMXmlDocument2
	: public HMXmlDocHandle
{
public:
	HMXmlDocument2();
	~HMXmlDocument2();
private:
#ifdef YG_USE_TINYXML
	static const int STORAGESIZE = 72+24;
#else
	static const int STORAGESIZE = 32836+124;
#endif // !YG_USE_TINYXML
	char		     fStorage__[STORAGESIZE];
};

NS_END(base)
// -------------------------------------------------------------------------

#endif /* __HM_XML_H__ */
