/* -------------------------------------------------------------------------
//	FileName	：	hm_xml.cpp
//	Creator		：	(zc)
//	CreateTime	：	2013-04-18 10:08:19
//	Description	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "hm_xml.h"

#ifdef YG_USE_TINYXML
	#include "tinyxml.h"		// for tinyxml
#else
	#include "rapidxml.hpp"		// for rapidxml
	class RdXmlNode		: public rapidxml::xml_node<char> {};
	class RdXmlAttribute: public rapidxml::xml_attribute<char> {};
	class RdXmlDocument	: public rapidxml::xml_document<char, RAPIDXML_STATIC_POOL_SIZE/2> {};
#endif // _DEBUG

#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



NS_BEGIN(base)
//////////////////////////////////////////////////////////////////////////
// HMXmlAttribute
HMXmlAttribute::HMXmlAttribute()
	: xmlAttr__(NULL)
{

}

/*
@func		: isValid
@brief		: 是否可用。
*/
bool HMXmlAttribute::isValid() const
{
	return (xmlAttr__ != NULL);
}

/*
@func		: isEqual
@brief		: 是否相同。
*/
bool HMXmlAttribute::isEqual(const HMXmlAttribute& other) const
{
	return (xmlAttr__ == other.xmlAttr__);
}

/*
@func		: Name
@brief		: 
*/
LPCSTR	HMXmlAttribute::getName()  const
{
	if (xmlAttr__ != NULL) {
#ifdef YG_USE_TINYXML
		return xmlAttr__->Name();
#else
		return xmlAttr__->name();
#endif
	}
	return NULL;
}

/*
@func		: Value
@brief		: 
*/
utf8string	HMXmlAttribute::getValue() const
{
	if (xmlAttr__ != NULL) {
#ifdef YG_USE_TINYXML
		return xmlAttr__->Value();
#else
		return xmlAttr__->value();
#endif
	}
	return NULL;
}

/*
@func		: Next
@brief		: 
*/
HMXmlAttribute HMXmlAttribute::nextAttribute() const
{
	HMXmlAttribute xmlAttribute;
	if (xmlAttr__ != NULL) {
#ifdef YG_USE_TINYXML
		xmlAttribute.xmlAttr__ = xmlAttr__->Next();
#else
		xmlAttribute.xmlAttr__ = static_cast<RdXmlAttribute*>(xmlAttr__->next_attribute());
#endif
	}
	return xmlAttribute;
}

//////////////////////////////////////////////////////////////////////////
// HMXmlNode
HMXmlNode::HMXmlNode()
	: xmlNode__(NULL)
{

}

/*
@func		: isValid
@brief		: 是否可用。
*/
bool HMXmlNode::isValid() const
{
	return (xmlNode__ != NULL);
}

/*
@func		: isEqual
@brief		: 是否相同。
*/
bool HMXmlNode::isEqual(const HMXmlNode& other) const
{
	return (xmlNode__ == other.xmlNode__);
}

/*
@func		: firstChild
@brief		:
*/
HMXmlNode HMXmlNode::firstChild(LPCSTR xmlTag/* = NULL*/) const
{
	HMXmlNode xmlNodeObj;
	if (xmlNode__) {
#ifdef YG_USE_TINYXML
		if (IsValidString(xmlTag)) {
			xmlNodeObj.xmlNode__ = xmlNode__->FirstChild(xmlTag);
		} else {
			xmlNodeObj.xmlNode__ = xmlNode__->FirstChild();
		}
#else
		xmlNodeObj.xmlNode__	= static_cast<RdXmlNode*>(xmlNode__->first_node(xmlTag));
#endif // _DEBUG
		
	}
	return xmlNodeObj;
}

/*
@func		: lastChild
@brief		:
*/
HMXmlNode HMXmlNode::lastChild(LPCSTR xmlTag/* = NULL*/) const
{
	HMXmlNode xmlNodeObj;
	if (xmlNode__) {
#ifdef YG_USE_TINYXML
		if (IsValidString(xmlTag)) {
			xmlNodeObj.xmlNode__ = xmlNode__->LastChild(xmlTag);
		} else {
			xmlNodeObj.xmlNode__ = xmlNode__->LastChild();
		}
#else
		xmlNodeObj.xmlNode__	= static_cast<RdXmlNode*>(xmlNode__->last_node(xmlTag));
#endif
	}
	return xmlNodeObj;
}

/*
@func		: previousSibling
@brief		:
*/
HMXmlNode HMXmlNode::previousSibling(LPCSTR xmlTag/* = NULL*/) const
{
	HMXmlNode xmlNodeObj;
	if (xmlNode__) {
#ifdef YG_USE_TINYXML
		if (IsValidString(xmlTag)) {
			xmlNodeObj.xmlNode__ = xmlNode__->PreviousSibling(xmlTag);
		} else {
			xmlNodeObj.xmlNode__ = xmlNode__->PreviousSibling();
		}
#else
		xmlNodeObj.xmlNode__	= static_cast<RdXmlNode*>(xmlNode__->previous_sibling(xmlTag));
#endif
	}
	return xmlNodeObj;
}

/*
@func		: nextSibling
@brief		: Navigate to a sibling node.
*/
HMXmlNode HMXmlNode::nextSibling(LPCSTR xmlTag/* = NULL*/) const
{
	HMXmlNode xmlNodeObj;
	if (xmlNode__) {
#ifdef YG_USE_TINYXML
		if (IsValidString(xmlTag)) {
			xmlNodeObj.xmlNode__ = xmlNode__->NextSibling(xmlTag);
		} else {
			xmlNodeObj.xmlNode__ = xmlNode__->NextSibling();
		}
#else
		xmlNodeObj.xmlNode__	= static_cast<RdXmlNode*>(xmlNode__->next_sibling(xmlTag));
#endif
	}
	return xmlNodeObj;
}

//////////////////////////////////////////////////////////////////////////
// HMXmlElement
/*
@func		: tagName
@brief		: 如果当前节点是element，则返回其标签名。
*/
LPCSTR HMXmlElement::tagName() const
{
#ifdef YG_USE_TINYXML
	if(xmlNode__ && NULL != xmlNode__->ToElement()) {
		return xmlNode__->Value();
	}
#else
	if (xmlNode__ && rapidxml::node_element == xmlNode__->type()) {
		return xmlNode__->name();
	}
#endif // YG_USE_TINYXML
	return NULL;
}

/*
@func		: isTagName
@brief		: 是否是指定的标签名。
*/
bool HMXmlElement::isTagName(LPCSTR pszTag)
{
	LPCSTR xmlTagName = tagName();
	if (IsValidString(xmlTagName)/* && IsValidString(pszTag)*/) {
		return 0 == hm_stricmp(xmlTagName, pszTag);
	}
	//
	return false;
}

/*
@func		: getText
@brief		: 如果当前节点含有TiXmlText节点，则返回相应的值，否则，返回NULL
			  宽字符版本则返回“”字符串
*/
utf8string HMXmlElement::getText() const
{
#ifdef YG_USE_TINYXML
	if (xmlNode__) {
		TiXmlElement* xmlElem = xmlNode__->ToElement();
		if (xmlElem) {
			return xmlElem->GetText();
		}
	}
#else
	if (xmlNode__ && rapidxml::node_element == xmlNode__->type()) {
		// ASSERT(FALSE);
		RdXmlNode* pChildNode = static_cast<RdXmlNode*>(xmlNode__->first_node());
		if (pChildNode 
			&& (pChildNode->type() == rapidxml::node_data 
			    || pChildNode->type() == rapidxml::node_cdata
				)
			) {
			return pChildNode->value();
		}
	}
#endif // YG_USE_TINYXML
	return NULL;
}

/*
@func		: attribute
@brief		:
*/
utf8string HMXmlElement::attribute(IN LPCSTR name, IN LPCSTR pszDefValue/* = NULL*/) const
{
#ifdef YG_USE_TINYXML
	if (xmlNode__) {
		TiXmlElement* xmlElem = xmlNode__->ToElement();
		if (xmlElem) {
			return xmlElem->Attribute(name);
		}
	}
#else
	if (xmlNode__ && rapidxml::node_element == xmlNode__->type()) {
		RdXmlAttribute* attribNode = static_cast<RdXmlAttribute*>(xmlNode__->first_attribute(name));
		if (attribNode){
			return attribNode->value();
		}
	}
#endif // YG_USE_TINYXML
	return pszDefValue;
}

/*
@func		: attributeInt
@brief		:
*/
int32 HMXmlElement::attributeInt(IN LPCSTR name, int32 nDefValue/* = 0*/) const
{
#ifdef YG_USE_TINYXML
	if (xmlNode__) {
		TiXmlElement* xmlElem = xmlNode__->ToElement();
		if (xmlElem) {
			xmlElem->Attribute(name, &nDefValue);
		}
	}
#else
	if (xmlNode__ && rapidxml::node_element == xmlNode__->type()) {
		RdXmlAttribute* attribNode = static_cast<RdXmlAttribute*>(xmlNode__->first_attribute(name));
		if (attribNode) {
			LPCSTR attribValue = attribNode->value();
			if (IsValidString(attribValue)) {
				nDefValue = atoi(attribValue);
			}
		}
	}
#endif // YG_USE_TINYXML
	//
	return nDefValue;
}

/*
@func		: firstChildElement
@brief		:
*/
HMXmlElement HMXmlElement::firstChildElement(LPCSTR xmlTag/* = NULL*/) const
{
	HMXmlElement xmlElemObj;
	if (xmlNode__) {
#ifdef YG_USE_TINYXML
		if (IsValidString(xmlTag)) {
			xmlElemObj.xmlNode__ = xmlNode__->FirstChildElement(xmlTag);
		} else {
			xmlElemObj.xmlNode__ = xmlNode__->FirstChildElement();
		}
#else
		RdXmlNode* _pXmlElem = static_cast<RdXmlNode*>(xmlNode__->first_node(xmlTag));
		if (_pXmlElem && rapidxml::node_element == _pXmlElem->type()) {
			xmlElemObj.xmlNode__ = _pXmlElem;
		}
#endif
	}
	return xmlElemObj;
}


/*
@func		: nextSiblingElement
@brief		: Navigate to a sibling node.
*/
HMXmlElement HMXmlElement::nextSiblingElement(LPCSTR xmlTag/* = NULL*/) const
{
	HMXmlElement xmlElemObj;
	if (xmlNode__) {
#ifdef YG_USE_TINYXML
		if (IsValidString(xmlTag)) {
			xmlElemObj.xmlNode__ = xmlNode__->NextSiblingElement(xmlTag);
		} else {
			xmlElemObj.xmlNode__ = xmlNode__->NextSiblingElement();
		}
#else
		RdXmlNode* _pXmlElem = static_cast<RdXmlNode*>(xmlNode__->next_sibling(xmlTag));
		if (_pXmlElem && rapidxml::node_element == _pXmlElem->type()) {
			xmlElemObj.xmlNode__ = _pXmlElem;
		}
#endif
	}
	return xmlElemObj;
}

/*
@func		: firstAttribute
@brief		: Sets an attribute of name to a given value. The attribute
			  will be created if it does not exist, or changed if it does.
*/
HMXmlAttribute HMXmlElement::firstAttribute() const
{
	HMXmlAttribute xmlAttribute;
#ifdef YG_USE_TINYXML
	if (xmlNode__) {
		TiXmlElement* xmlElem = xmlNode__->ToElement();
		if (xmlElem) {
			xmlAttribute.xmlAttr__ = xmlElem->FirstAttribute();
		}
	}
#else
	if (xmlNode__ && rapidxml::node_element == xmlNode__->type()) {
		xmlAttribute.xmlAttr__ = static_cast<RdXmlAttribute*>(xmlNode__->first_attribute());
	}
#endif
	//
	return xmlAttribute;
}

//////////////////////////////////////////////////////////////////////////
// HMXmlDocHandle
HMXmlDocHandle::HMXmlDocHandle(
#ifdef YG_USE_TINYXML
	TiXmlDocument* xmlDoc
#else
	RdXmlDocument* xmlDoc
#endif // !YG_USE_TINYXML
) : xmlDoc__(xmlDoc), xmlError__(false)
{

}

/*
@func		: isValid
@brief		: 是否可用。
*/
bool HMXmlDocHandle::isValid() const
{
	return (xmlDoc__ != NULL);
}

/*
@func		: isEqual
@brief		: 是否相同。
*/
bool HMXmlDocHandle::isEqual(const HMXmlDocHandle& other) const
{
	return (xmlDoc__ == other.xmlDoc__);
}

/*
@func		: isError
@brief		:
*/
bool HMXmlDocHandle::isError() const
{
	return xmlError__;
}

/*
@func		: rootElement
@brief		:
*/
HMXmlElement HMXmlDocHandle::rootElement(LPCSTR xmlTag/* = NULL*/) const
{
	HMXmlElement xmlRoot;
#ifdef YG_USE_TINYXML
	if (IsValidString(xmlTag)) {
		xmlRoot.xmlNode__ = xmlDoc__->FirstChildElement(xmlTag);
	} else {
		xmlRoot.xmlNode__ = xmlDoc__->RootElement();
	}
#else
		xmlRoot.xmlNode__ = static_cast<RdXmlNode*>(xmlDoc__->first_node(xmlTag));
#endif
	//
	return xmlRoot;
}

/*
@func		: loadFromString
@brief		:
*/
bool HMXmlDocHandle::loadFromString(IN const utf8string xmlString)
{
#ifdef YG_USE_TINYXML
	xmlError__ = true;
	if (IsValidString(xmlString)) {
		xmlDoc__->Parse((const char*)xmlString, 0, TIXML_ENCODING_UTF8);
		xmlError__ = xmlDoc__->Error();
	}
#else
	char*p = xmlDoc__->allocate_string(xmlString);
	xmlError__ = false;
	try {
		xmlDoc__->parse<rapidxml::parse_trim_whitespace|rapidxml::parse_validate_closing_tags|rapidxml::parse_no_element_values>((char*)p);
	} catch (rapidxml::parse_error* e) {
#ifdef _DEBUG
		e->what();
#else
		(void)e;
#endif // _DEBUG
		xmlError__ = true;
	} catch (...) {
		xmlError__ = true;
	}
#endif
	//
	return !xmlError__;
}

//////////////////////////////////////////////////////////////////////////
// HMXmlDocument
HMXmlDocument::HMXmlDocument()
#ifdef YG_USE_TINYXML
	: HMXmlDocHandle(new TiXmlDocument)
#else
	: HMXmlDocHandle(new RdXmlDocument)
#endif // !YG_USE_TINYXML
{

}

HMXmlDocument::~HMXmlDocument()
{
	if (xmlDoc__) {
		delete xmlDoc__;
	}
}

//////////////////////////////////////////////////////////////////////////
// HMXmlDocument2
HMXmlDocument2::HMXmlDocument2()
	: HMXmlDocHandle(NULL)
{
#ifdef YG_USE_TINYXML
	// sizeof(TiXmlDocument) = 72
	char error_msg[STORAGESIZE < sizeof(TiXmlDocument) ? -1 : 1];
	error_msg[0];
	#ifdef new
		#pragma push_macro("new")
		#undef new
		xmlDoc__ = ::new (fStorage__) TiXmlDocument();
		#pragma pop_macro("new")
	#else
		xmlDoc__ = ::new (fStorage__) TiXmlDocument();
	#endif // new
#else
	// sizeof(RdXmlDocument) = 32836
	char error_msg[STORAGESIZE < sizeof(RdXmlDocument) ? -1 : 1];
	error_msg[0];
	#ifdef new
		#pragma push_macro("new")
		#undef new
		xmlDoc__ = ::new (fStorage__) RdXmlDocument();
		#pragma pop_macro("new")
	#else
		xmlDoc__ = ::new (fStorage__) RdXmlDocument();
	#endif // new
#endif // !YG_USE_TINYXML
}

HMXmlDocument2::~HMXmlDocument2()
{
#ifdef YG_USE_TINYXML
	xmlDoc__->~TiXmlDocument();
#else
	xmlDoc__->~RdXmlDocument();
#endif // !YG_USE_TINYXML
}

NS_END(base)


// -------------------------------------------------------------------------
