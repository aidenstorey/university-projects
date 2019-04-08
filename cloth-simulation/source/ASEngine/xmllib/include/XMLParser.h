#ifndef GUARD_XMLPARSER_H_1385536898
#define GUARD_XMLPARSER_H_1385536898

enum
{
	PARSENODE_XML_DOCUMENT,
	PARSENODE_XML_CHAR,
	PARSENODE_XML_S,
	PARSENODE_XML_NAMECHAR,
	PARSENODE_XML_NAME,
	PARSENODE_XML_NAMES,
	PARSENODE_XML_NMTOKEN,
	PARSENODE_XML_NMTOKENS,
	PARSENODE_XML_ENTITYVALUE,
	PARSENODE_XML_ATTVALUE,
	PARSENODE_XML_SYSTEMLITERAL,
	PARSENODE_XML_PUBIDLITERAL,
	PARSENODE_XML_PUBIDCHAR,
	PARSENODE_XML_CHARDATA,
	PARSENODE_XML_COMMENT,
	PARSENODE_XML_PI,
	PARSENODE_XML_PITARGET,
	PARSENODE_XML_CDSECT,
	PARSENODE_XML_CDSTART,
	PARSENODE_XML_CDATA,
	PARSENODE_XML_CDEND,
	PARSENODE_XML_PROLOG,
	PARSENODE_XML_XMLDECL,
	PARSENODE_XML_VERSIONINFO,
	PARSENODE_XML_EQ,
	PARSENODE_XML_VERSIONNUM,
	PARSENODE_XML_MISC,
	PARSENODE_XML_DOCTYPEDECL,
	PARSENODE_XML_DECLSEP,
	PARSENODE_XML_INTSUBSET,
	PARSENODE_XML_MARKUPDECL,
	PARSENODE_XML_EXTSUBSET,
	PARSENODE_XML_EXTSUBSETDECL,
	PARSENODE_XML_SDDECL,
	PARSENODE_XML_ELEMENT,
	PARSENODE_XML_STAG,
	PARSENODE_XML_ATTRIBUTE,
	PARSENODE_XML_ETAG,
	PARSENODE_XML_CONTENT,
	PARSENODE_XML_EMPTYELEMTAG,
	PARSENODE_XML_ELEMENTDECL,
	PARSENODE_XML_CONTENTSPEC,
	PARSENODE_XML_CHILDREN,
	PARSENODE_XML_CP,
	PARSENODE_XML_CHOICE,
	PARSENODE_XML_SEQ,
	PARSENODE_XML_MIXED,
	PARSENODE_XML_ATTLISTDECL,
	PARSENODE_XML_ATTDEF,
	PARSENODE_XML_ATTTYPE,
	PARSENODE_XML_STRINGTYPE,
	PARSENODE_XML_TOKENIZEDTYPE,
	PARSENODE_XML_ENUMERATEDTYPE,
	PARSENODE_XML_NOTATIONTYPE,
	PARSENODE_XML_ENUMERATION,
	PARSENODE_XML_DEFAULTDECL,
	PARSENODE_XML_CONDITIONALSECT,
	PARSENODE_XML_INCLUDESECT,
	PARSENODE_XML_IGNORESECT,
	PARSENODE_XML_IGNORESECTCONTENTS,
	PARSENODE_XML_IGNORE,
	PARSENODE_XML_CHARREF,
	PARSENODE_XML_REFERENCE,
	PARSENODE_XML_ENTIYTREF,
	PARSENODE_XML_PEREFERENCE,
	PARSENODE_XML_ENTITYDECL,
	PARSENODE_XML_GEDECL,
	PARSENODE_XML_PEDECL,
	PARSENODE_XML_ENTITYDEF,
	PARSENODE_XML_PEDEF,
	PARSENODE_XML_EXTERNALID,
	PARSENODE_XML_NDATADECL,
	PARSENODE_XML_TEXTDECL,
	PARSENODE_XML_EXTPARSEDENT,
	PARSENODE_XML_ENCODINGDECL,
	PARSENODE_XML_ENCNAME,
	PARSENODE_XML_NOTATIONDECL,
	PARSENODE_XML_PUBLICID,
	PARSENODE_XML_LETTER,
	PARSENODE_XML_BASECHAR,
	PARSENODE_XML_IDEOGRAPHIC,
	PARSENODE_XML_COMBININGCHAR,
	PARSENODE_XML_DIGIT,
	PARSENODE_XML_EXTENDER,

	PARSENODE_XML_COUNT
};

// Library includes
#include <string>

// Local includes
#include "ParseNodeManager.h"

class CXMLStringContent
{
public:
	CXMLStringContent();
	CXMLStringContent( char *pszSource );
	~CXMLStringContent();

	void SetString( char *pszSourceString );
	char *raw();
	char *encoded();

private:
	char *m_pszRawString;
	char *m_pszEncodedString;
};

class CXMLParser : public CParser
{
public:
	CXMLParser();
	~CXMLParser();

	bool	InitParser(char *pszSource, long nSourceLength, bool bKeepParseTree=true, bool Validate=true /* If validate is true, KeepTree will be forced to true */); // Source MUST have at least one terminating character to allow detection of data truncation, and it should be zero
	char	*Parse(char *pszSource, long nSourceLength, bool bKeepParseTree=true, bool Validate=true); // Source MUST have at least one terminating character to allow detection of data truncation, and it should be zero
	char	*ParseFile( const char *_pszSourceFile, bool bKeepParseTree=true, bool Validate=true);
	char	*Parse(); // Source MUST have at least one terminating character to allow detection of data truncation, and it should be zero
	bool	ValidateXML();	// Runs post-parse validation of the generated tree to ensure the XML conforms to the external rules such as case sensitive tag matching (requires PARSENODE_XML_ELEMENT, PARSENODE_XML_STAG, PARSENODE_XML_ETAG and PARSENODE_XML_NAME to be returned in the parse tree)
private:
	bool	ValidateElement(CParseNode	*pNodeScan);
public:
	int		GetContentsArray( const CParseNode *pStartNode, char **ppszContentsArray, int nNumElements, bool bFailOnNull, bool bTreatEmptyStringsNull, char *pszChildElementName);	// Extracts into a defined size char array the children of a node that match the element name
	int		GetChildContentsArray( const CParseNode *pChildNode, int nNumElements, bool bFailOnNull, bool bTreatEmptyStringsNull, char **ppszContentsArray, char **ppszElementsArray=0);	// Extracts into a defined size char array the children of a node regardless of the element names
	int		GetChildContentsArray( const CParseNode *pChildNode, int nNumElements, bool bFailOnNull, bool bTreatEmptyStringsNull, char ***ppszContentsArray, char ***ppszElementsArray=0);	// Allocate and extract into a defined size char array the children of a node regardless of the element names
	int		GetChildContentsArray( const CParseNode *pChildNode, bool bFailOnNull, bool bTreatEmptyStringsNull, char ***ppszContentsArray, char ***ppszElementsArray=0);	// Allocate and extracts into an array the children of a node regardless of the element names.  Dynamically sized the array to the number of elements
	const CParseNode	*LocateElement( const CParseNode *pStartNode, const char *pszElementName, ...  /* Last name must be zero */) const;	// Jump to the first element matching the name, and any number of children
	const CParseNode	*LocateFirstChildElement( const CParseNode *pStartNode, char *pszElementName);	// Jump to the first child element matching the name
	const CParseNode	*LocateNextElement( const CParseNode *pStartNode, char *pszElementName, ...  /* Last name must be zero */) const;// Jump to the next element matching the name within the current element
																													//	level.  Regardless of the nest level, this function only uses the final
																													//	so a caller can reuse the parameters from a LocateElement call
	char		*GetElementContent( const CParseNode *pStartNode, bool bReturnEmptyStringAsNull = false) const;	// Get the char pointer to the data - you should have called the base class 'TerminateStrings' on type PARSENODE_XML_CONTENT before doing this to make
															// the data useable

	const CParseNode	*GetElementContentNode( const CParseNode *pStartNode, bool bReturnNullAsZero=false) const;

	char		*GetElementName( const CParseNode *pStartNode ); // Get the char pointer to the name of the element - you should have called the base class 'TerminateStrings' on type PARSENODE_XML_NAME before doing this to make the name useable

	bool		TestElementParameter( const CParseNode *pElement, const char *pszAttribute, const char*pszAttValue, bool bAttributeCaseSensitive=false, bool bAttrValueCaseSensitive=false); // bool test to check if a specific attr/value pair exist
	char		*GetElementParameter( const CParseNode *pElement, const char *pszAttribute, bool bAttributeCaseSensitive, bool bTerminateString = false /* Strip XML quotes before returning string */) const;

	void		Reset();

//	static unsigned char *XMLEncode(unsigned char *pszSource, bool bReturnOriginalOnUnchanged = false /* Returns original */ );	/* Helper function to translate text into XML safe data */
//	static unsigned char *XMLDecode(unsigned char *pszSource, bool bTranslateInline = false /* decode directly into pszSource */);	/* Helper function to translate text from XML safe data to its original form */

	template <class dataType> static dataType *XMLEncode(dataType *pszSource, bool bReturnOriginalOnUnchanged = false /* Returns original */ )//;	/* Helper function to translate text into XML safe data */
	{
		dataType *pszEncodeData = NULL;
		int nSourcePos = 0;
		int nDestPos = 0;
		int nTargetSize = 0;

		if( pszSource != 0 )
		{
			while( pszSource[nSourcePos] != 0 )
			{
				if( pszSource[nSourcePos] == '&' ) nTargetSize += ( sizeof("&amp;") - 1 );
				else if( pszSource[nSourcePos] == '<' ) nTargetSize += ( sizeof("&lt;") - 1 );
				else if( pszSource[nSourcePos] == '>' ) nTargetSize += ( sizeof("&gt;") - 1 );
				else if( pszSource[nSourcePos] == '\'' ) nTargetSize += ( sizeof("&apos;") - 1 );
				else if( pszSource[nSourcePos] == '"' ) nTargetSize += ( sizeof("&quot;") - 1 );
				else nTargetSize++;
				nSourcePos++;
			}

			if( bReturnOriginalOnUnchanged && nTargetSize == nSourcePos )
			{
				pszEncodeData = pszSource;
			}
			else
			{
				pszEncodeData = new dataType[nTargetSize + 1];

				nSourcePos = 0;
				while( pszSource[nSourcePos] != 0 )
				{
					if( pszSource[nSourcePos] == '&' ) {memcpy(pszEncodeData + nDestPos, "&amp;", sizeof("&amp;") - 1);nDestPos += ( sizeof("&amp;") - 1 );}
					else if( pszSource[nSourcePos] == '<' ) {memcpy(pszEncodeData + nDestPos, "&lt;", sizeof("&lt;") - 1);nDestPos += ( sizeof("&lt;") - 1 );}
					else if( pszSource[nSourcePos] == '>' ) {memcpy(pszEncodeData + nDestPos, "&gt;", sizeof("&gt;") - 1);nDestPos += ( sizeof("&gt;") - 1 );}
					else if( pszSource[nSourcePos] == '\'' ) {memcpy(pszEncodeData + nDestPos, "&apos;", sizeof("&apos;") - 1);nDestPos += ( sizeof("&apos;") - 1 );}
					else if( pszSource[nSourcePos] == '"' ) {memcpy(pszEncodeData + nDestPos, "&quot;", sizeof("&quot;") - 1);nDestPos += ( sizeof("&quot;") - 1 );}
					else pszEncodeData[nDestPos++] = pszSource[nSourcePos];
					nSourcePos++;
				}
				pszEncodeData[nDestPos] = 0;
			}
		}

		return(pszEncodeData);
	};

	template< typename T > static T XMLEncode( const T& _krstrSource )
	{
		std::string strEncodedString = _krstrSource;

		FindAndReplace( strEncodedString, "&", "&amp;" );
		FindAndReplace( strEncodedString, "<", "&lt;" );
		FindAndReplace( strEncodedString, ">", "&gt;" );
		FindAndReplace( strEncodedString, "'", "&apos;" );
		FindAndReplace( strEncodedString, "\"", "&quot;" );

		return ( strEncodedString );
	};

	template <class dataType> static dataType *XMLDecode(dataType *pszSource, bool bTranslateInline = false /* decode directly into pszSource */)//;	/* Helper function to translate text from XML safe data to its original form */
	{
		int nSourcePos = 0;
		int nDestPos = 0;
		dataType *pszDecodeData;
		if( ( pszDecodeData = pszSource ) != NULL )
		{
			if( !bTranslateInline ) pszDecodeData = new dataType[strlen((char *)pszSource) + 1];
			if( pszDecodeData != 0 )
			{
				while( pszSource[nSourcePos] != 0 )
				{
					if( strnicmp( (char *)pszSource+nSourcePos, "&amp;", sizeof("&amp;") - 1 ) == 0 )
					{
						pszDecodeData[nDestPos++] = '&';
						nSourcePos += ( sizeof("&amp;") - 1 );
					}
					else if( strnicmp( (char *)pszSource+nSourcePos, "&lt;", sizeof("&lt;") - 1 ) == 0 )
					{
						pszDecodeData[nDestPos++] = '<';
						nSourcePos += ( sizeof("&lt;") - 1 );
					}
					else if( strnicmp( (char *)pszSource+nSourcePos, "&gt;", sizeof("&gt;") - 1 ) == 0 )
					{
						pszDecodeData[nDestPos++] = '>';
						nSourcePos += ( sizeof("&gt;") - 1 );
					}
					else if( strnicmp( (char *)pszSource+nSourcePos, "&apos;", sizeof("&apos;") - 1 ) == 0 )
					{
						pszDecodeData[nDestPos++] = '\'';
						nSourcePos += ( sizeof("&apos;") - 1 );
					}
					else if( strnicmp( (char *)pszSource+nSourcePos, "&quot;", sizeof("&quot;") - 1 ) == 0 )
					{
						pszDecodeData[nDestPos++] = '"';
						nSourcePos += ( sizeof("&quot;") - 1 );
					}
					else pszDecodeData[nDestPos++] = pszSource[nSourcePos++];
				}
				pszDecodeData[nDestPos++] = 0;
			}
		}

		return(pszDecodeData);
	};


private:
//	bool		bReturnNodesUserSet;
	bool		bParserInitialized;

	char*		m_pszLoadedSource;

	char		*m_pszSource;
	long		m_nSourceLength;
	bool		bKeepParseTree;

	char *ParseToken_XML_document(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_char(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_s(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_namechar(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_name(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_names(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_nmtoken(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_nmtokens(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_entityvalue(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_attvalue(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_systemliteral(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_pubidliteral(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_pubidchar(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_chardata(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_comment(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_pi(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_pitarget(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_cdsect(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_cdstart(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_cdata(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_cdend(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_prolog(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_xmldecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_versioninfo(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_eq(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_versionnum(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_misc(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_doctypedecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_declsep(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_intsubset(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_markupdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_extsubset(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_extsubsetdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_sddecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_element(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_stag(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_attribute(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_etag(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_content(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_emptyelemtag(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_elementdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_contentspec(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_children(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_cp(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_choice(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_seq(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_mixed(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_attlistdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_attdef(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_atttype(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_stringtype(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_tokenizedtype(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_enumeratedtype(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_notationtype(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_enumeration(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_defaultdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_conditionalsect(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_includesect(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_ignoresect(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_ignoresectcontents(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_ignore(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_charref(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_reference(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_entityref(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_pereference(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_entitydecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_gedecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_pedecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_entitydef(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_pedef(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_externalid(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_ndatadecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_textdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_extparsedent(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_encodingdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_encname(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_notationdecl(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_publicid(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_letter(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_basechar(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_ideographic(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_combiningchar(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_digit(char *pszTokenStart, CParseNode *pParent);
	char *ParseToken_XML_extender(char *pszTokenStart, CParseNode *pParent);

};

#endif // GUARD_XMLPARSER_H_1385536898
