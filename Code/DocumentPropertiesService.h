/**
* DocumentPropertiesService.h
* 
*  @file		   DocumentPropertiesService.h
*  @author         Vitaly Artemov (vitaly_artemov@mcafee.com)
*				   Sharon Sahar (Sharon_Sahar@mcafee.com)
*  @par Module:
*                  AgentCommon
*  @par Company:
*				    McAfee ( http://www.mcafee.com )      
*
*  @par Version History:
*
*/

#ifndef _DOCUMENT_PROPERTIES_RETRIEVER_H_
#define _DOCUMENT_PROPERTIES_RETRIEVER_H_

#include <string>
#include <map>
#include <ole2.h>
#include <ClassificationEngine\ClassificationStructures.h>
#include "../AgentInternalServices/FileSharingService.h"
#include "FileInfo.h"

#pragma comment( lib , "AgentCommon.lib" )
typedef std::map<std::wstring, std::wstring>	SimpleStringMap;
#include <unzip.h>

#import "MSXML3.dll" named_guids
//using namespace MSXML2;

//forward declaration of SXMPMeta
template <class tStringObj>
class TXMPMeta;
typedef class TXMPMeta <std::string> SXMPMeta;

using namespace std;
class FileContainer;
// Property Names Definitions
#define REVISION_PROP_NAME L"Revision Number"
#define STATUS_PROP_NAME L"Status"
#define APPLICATION_PROP_NAME L"Application"
#define PRESENTATIONFORMAT_PROP_NAME L"Presentation Format"
#define SLIDES_PROP_NAME L"Slides"
#define MANAGER_PROP_NAME L"Manager"
#define COMPANY_PROP_NAME L"Company"
#define HYPERLINKBASE_PROP_NAME L"Hyperlink Base"
#define SOURCE_PROP_NAME L"Source"
#define ANYPROPERTY_PROP_NAME L"AnyProperty"

typedef std::list<std::pair<std::wstring, std::string>> StringPairList;

struct pidsiStruct {
	WCHAR *name;
	long pidsi;
};

#define IGNORE_PROPERTY_VALUE L"[IGNORE]"

struct DLPCustomProperties
{
	std::wstring mClassifications;
	std::wstring mClassifier;
	std::wstring mDate;
	std::wstring mAutoClassifications;
	std::wstring mManualVersion;
	std::wstring mAutoVersion;
};

class DocumentPropertiesService
{
public:

	DocumentPropertiesService();

	void open( FileSharingService& afileSharingService, std::wstring& aAgentTempFolder );

	void close();

	bool getDocumentProperties(
		std::wstring& aFilePath,
		std::wstring& aLogicalFilePath,
		UINT32 aMountIndex,
		UINT32 aSafebootMountIndex,
		BOOL aIsLocalDrive,
		UINT32 aMaxFileSizeForExtraction,
		CSF::DocumentPropertiesMap& aDocumentPropertiesMap,
		UINT32& aCreationDate,
		UINT32& aModificationDate,
		UINT32& aFileSize );

	static void populateProperties(
		const SimpleStringMap	&aSourceMap,
		CSF::DocumentPropertiesMap& aDocumentPropertiesMap);

	BOOL	extractFileInformation(
		const std::wstring& filePath,
		const std::wstring& logicalFilePath,
		UINT32 aMountIndex, 
		UINT32 aSafebootMountIndex, 
		BOOL aIsLocalDrive,
		UINT32& aCreationDate,
		UINT32& aModificationDate,
		UINT32& aFileSize );

	static BOOL	extractFileInformation(
		FileContainer	&fileContainer,
		UINT32& aCreationDate,
		UINT32& aModificationDate,
		UINT32& aFileSize );

	std::wstring getFileOwner( AutoFileInfo aFileInfo );

	bool setDlpCustomProperties( 
		const std::wstring& aFilePath,
		const DLPCustomProperties& aCustomProperties,
		ManualClassificationError& aError );

	bool getDlpCustomProperties( 
		const std::wstring& aFilePath,
		HANDLE aFileHandle,
		DLPCustomProperties& aCustomProperties,
		ManualClassificationError& aError );

	bool setDlpXmpProperties( 
		const std::wstring& aFilePath,
		const DLPCustomProperties& aCustomProperties,
		ManualClassificationError& aError );

	bool getDlpXmpProperties( 
		const std::wstring& aFilePath,
		HANDLE& aFileHandle,
		DLPCustomProperties& aCustomProperties,
		ManualClassificationError& aError );

private:


	std::wstring convertDate( 
		const std::wstring &aString, 
		bool removeLeadingZeroes = true, 
		WCHAR newSeparator = L'/' );

	bool	isDigit(
		const WCHAR	wc );
	
	bool	isDateFormat(
		const std::wstring &aString );


	bool getDocumentPropertiesFromFile( 
		std::wstring& filePath,
		std::wstring& logicalFilePath, 
		UINT32 aMountIndex, 
		UINT32 aSafebootMountIndex, 
		BOOL aIsLocalDrive,
		UINT32 aMaxFileSizeForExtraction,
		CSF::DocumentPropertiesMap& aDocumentPropertiesMap,
		UINT32& aCreationDate,
		UINT32& aModificationDate,
		UINT32& aFileSize );

	void populateBuiltInProperties(
		IPropertySetStorage* aPropertySetStorage,
		REFFMTID aFmtid,
		CSF::DocumentPropertiesMap& aDocumentPropertiesMap);

	void populateCustomProperties(
		IPropertySetStorage* aPropertySetStorage,
		CSF::DocumentPropertiesMap& aDocumentPropertiesMap);

	HKEY GetHKCRKey(
		WCHAR* pszKeyString, 
		WCHAR* pszPart );

	void addPropVariantToMap(
		PROPVARIANT* pPropVar,
		WCHAR* aName,
		WORD aCodePage,
		CSF::DocumentPropertiesMap& aDocumentPropertiesMap );

	bool getUnicodeXMLFromZipFile( 
		HZIP &hZIP, 
		ZIPENTRY &zipItem,  
		int zipItemIndex, 
		bstr_t& aXML );

	std::wstring getNodeValue( 
		MSXML2::IXMLDOMNodePtr pParentXmlNode, 
		std::wstring nodeName );

	bool getPropertiesFromZipFile( 
		HANDLE hZipFile,
		CSF::DocumentPropertiesMap &propertiesMap,
		FileContainer &fileContainer );

	bool getXMLFromZipFile( HZIP &hZIP, ZIPENTRY &zipItem, int zipItemIndex, _bstr_t& aXML );

	bool addContentTypesCustomPropsInfo( HZIP hZIP, StringPairList& aCustomXmls );

	bool addRelsCustomPropsInfo( HZIP hZIP, StringPairList& aCustomXmls );

	MSXML2::IXMLDOMElementPtr createCustomPropertyNode( 
		MSXML2::IXMLDOMDocumentPtr aXmlDOMObject, 
		const wchar_t* aNamespaceURI,
		const std::wstring& aName, 
		const std::wstring& aValue,
		int aPid );

	void updateCustomPropertyNode( 
		MSXML2::IXMLDOMDocumentPtr aXmlDOMObject, 
		MSXML2::IXMLDOMNodePtr aPropertiesNode,
		const std::wstring& aName, 
		const std::wstring& aValue,
		int& aPid );

	bool getHighestID( 
		MSXML2::IXMLDOMNodePtr aPropertiesNode, 
		LPCWSTR aNodeName, 
		LPCWSTR aNodeIDAttribute, 
		LPCWSTR aIDPrefix,
		int& aHighestID );

	bool createCustomXmls( 
		HZIP hZIP, 
		StringPairList& aCustomXmls, 
		const DLPCustomProperties& aCustomPropertiesValues );

	void setXmpProperty( 
		const std::wstring& aName, 
		const std::wstring& aValue, 
		SXMPMeta& aXMPMeta );

	void getXmpProperty( 
		const std::wstring& aName, 
		std::wstring& aValue, 
		const SXMPMeta& aXMPMeta );

	bool isPDF( const std::wstring& aFilePath, HANDLE aFileHandle );

	std::wstring findCustomProperty( const std::string& aContent, LPCSTR aPrefix, LPCSTR aSuffix );

	bool readCustomPropertiesFromPDF(
		const std::wstring& aFilePath,
		HANDLE aFileHandle,
		DLPCustomProperties& aCustomProperties,
		ManualClassificationError& aError );

	ZRESULT AddFileToZip(const TCHAR *zipfn, const TCHAR *zename, const TCHAR *zefn, char* inBuf = 0, int inBufLen = 0);

	bool openFileExclusively( 
		const std::wstring& aFilePath, 
		ManualClassificationError& aError );

	bool copyFileToTemp( 
		const std::wstring& aFilePath, 
		ManualClassificationError& aError, 
		std::wstring& aTempFileName );

	void addPropertyToWrite( 
		PROPSPEC* aPropsec, 
		PROPVARIANT* aPropvarWrite, 
		int& aIdx, 
		LPWSTR aName, 
		const std::wstring& aValue );

	bool setDlpStorageCustomProperties( 
		const std::wstring& aFilePath,
		const DLPCustomProperties& aCustomProperties );

	bool getDlpStorageCustomProperties( 	
		const std::wstring& aFilePath,
		HANDLE aFileHandle,
		DLPCustomProperties& aCustomProperties );

	//Array of PIDSI's you are interested in from SummaryInformation Property Set.
	static pidsiStruct m_pidsiArr[];

	// Array of PIDSI's you are interested in from DocSummaryInformation Property Set.
	static pidsiStruct m_pidsiArrDocSummary[];

	static pidsiStruct m_pidsiArrMediaInfoSummary[]; 

	FileSharingService* mFileSharingService;

	std::wstring mAgentTempFolder;
};

#endif _DOCUMENT_PROPERTIES_RETRIEVER_H_
