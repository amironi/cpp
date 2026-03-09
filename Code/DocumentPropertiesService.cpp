/**
* DocumentPropertiesService.cpp
* 
*  @file		   DocumentPropertiesService.cpp
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

#include <Framework/FrameworkExport.h>
#include "DocumentPropertiesService.h"
#include <Common/FileSystemUtils.h>
#include <Common/CILockBytesOnFileHandle.h>
#include "Aclapi.h"
#include "FileSystemUtils.h"
#include "StringUtilities.h"
#include "RegistryUtils.h"
#include <Common/CommonRegistry.h>

#include <string>
#include <sstream>
#include <windows.h>
#include <time.h>
#include <zip.h>
#include <Framework/FileUnlinker.h>

#include "../AgentInternalServices/FileSharingService.h"

#include "../../../Probes/Drivers/MiniFilter/UserLib/FileSharing/FileSharingObject.h"
#include "../../../Framework/FileContainer.h"

//XMP defines
// Must be defined to instantiate template classes
#define TXMP_STRING_TYPE std::string 
// Must be defined to give access to XMPFiles
#define XMP_INCLUDE_XMPFILES 1 
// Ensure XMP templates are instantiated
#include "public/include/XMP.incl_cpp"
// Provide access to the API
#include "public/include/XMP.hpp"
#include "source/XMPFiles_IO.hpp"
#include "public/include/XMP_const.h"
#include "..\ManualClassificationService\MCCommonDef.h"

#define FILE_SIZE_CEILING_VALUE 0xFFFFFFFE

using namespace CSF;

DocumentPropertiesService::DocumentPropertiesService() :
	mFileSharingService( NULL )
{
}

void DocumentPropertiesService::populateProperties(
	const SimpleStringMap	&aSourceMap,
	CSF::DocumentPropertiesMap& aDocumentPropertiesMap)
{
	for( auto iter = aSourceMap.begin(); iter != aSourceMap.end(); ++iter)
		aDocumentPropertiesMap[iter->first].push_back(iter->second);
}

bool XMPErrorCallbackProc( void* context, XMP_StringPtr filePath, XMP_ErrorSeverity severity, XMP_Int32 cause, XMP_StringPtr message )
{
	//throw XMP_Error exception
	return false;
}

void DocumentPropertiesService::open( FileSharingService& aFileSharingService, std::wstring& aAgentTempFolder )
{
	mFileSharingService = &aFileSharingService;
	mAgentTempFolder = aAgentTempFolder;

	std::wstring agentPath;
	RegistryUtils::RegistryGetStringValue( DLP_REG_PATH_INSTALLED_DIR, agentPath );

	std::string agentPathA;
	StringUtils::wstring2string( agentPathA, agentPath );

	if( !SXMPMeta::Initialize() ||
		!SXMPFiles::Initialize( agentPathA.c_str(), "PDF_Handler.xpi") )
	{
		OLOG( OLoggingManager::OSEV_CRITICAL, L"Failed to initialize XMP" );
	}

	SXMPFiles::SetDefaultErrorCallback( XMPErrorCallbackProc );
}

void DocumentPropertiesService::close()
{
	SXMPFiles::Terminate();
	SXMPMeta::Terminate();
}

bool DocumentPropertiesService::getDocumentProperties(
  std::wstring& aFilePath,
  std::wstring& aLogicalFilePath,
  UINT32 aMountIndex,
  UINT32 aSafebootMountIndex,
  BOOL aIsLocalDrive,
  UINT32 aMaxFileSizeForExtraction,
  DocumentPropertiesMap& aDocumentPropertiesMap,
  UINT32& aCreationDate,
  UINT32& aModificationDate,
  UINT32& aFileSize )
{
	try
	{
		if( !getDocumentPropertiesFromFile( 
			aFilePath, 
			aLogicalFilePath, 
			aMountIndex,
			aSafebootMountIndex,
			aIsLocalDrive,
			aMaxFileSizeForExtraction,
			aDocumentPropertiesMap,
			aCreationDate,
			aModificationDate,
			aFileSize ) )
		{
			OERROR_FILE_TRACKER2(
				"Unable to get document properties from  %s" , aFilePath.c_str() );
			return false;
		}
	}
	catch(...)
	{
		OERROR_FILE_TRACKER2(
			"Exception was thrown during document properties extraction from %s", aFilePath.c_str() );

		return false;
	}
			
	return true;
}

pidsiStruct DocumentPropertiesService::m_pidsiArr[] = {
	{SUBJECTS_PROP_NAME,          PIDSI_SUBJECT}, 
	{TITLES_PROP_NAME,            PIDSI_TITLE}, 
	{AUTHORS_PROP_NAME,   PIDSI_AUTHOR},
	{KEYWORDS_PROP_NAME,         PIDSI_KEYWORDS},
	{COMMENTS_PROP_NAME,         PIDSI_COMMENTS},
	{TEMPLATES_PROP_NAME,         PIDSI_TEMPLATE},
	{LASTSAVEDBY_PROP_NAME,       PIDSI_LASTAUTHOR},
	{L"Revision Number",  PIDSI_REVNUMBER},
	{L"Edit Time",        PIDSI_EDITTIME}, 
	{L"Last Printed",     PIDSI_LASTPRINTED},
	{L"Date Created",          PIDSI_CREATE_DTM},
	{L"Date Last Saved",       PIDSI_LASTSAVE_DTM},
	{L"Pages",       PIDSI_PAGECOUNT},
	{L"Word Count",       PIDSI_WORDCOUNT},
	{L"Character Count",       PIDSI_CHARCOUNT},
	{L"Thumbnail",        PIDSI_THUMBNAIL},
	{L"Application Name",          PIDSI_APPNAME}, 
	{SECURITY_PROP_NAME,     PIDSI_DOC_SECURITY},
	{0, 0}
};

pidsiStruct DocumentPropertiesService::m_pidsiArrDocSummary[] = {
	{CATEGORIES_PROP_NAME,        PIDDSI_CATEGORY},  // MIcorsoft bug. Category is always under PIDDSI_PRESFORMAT and Presentation Format is under PIDDSI_CATEGORY
	{L"Presentation Format",      PIDDSI_PRESFORMAT},
	{L"Byte Count",      PIDDSI_BYTECOUNT},
	{L"Line Count",      PIDDSI_LINECOUNT},
	{L"Paragraph Count",       PIDDSI_PARCOUNT},
	{L"Slides",     PIDDSI_SLIDECOUNT},
	{L"Notes",      PIDDSI_NOTECOUNT},
	{L"Hidden Count",	PIDDSI_HIDDENCOUNT},
	{L"Multimedia Clips",    PIDDSI_MMCLIPCOUNT}, 
	{L"Scale",			PIDDSI_SCALE}, 
	{L"Heading Pair",    PIDDSI_HEADINGPAIR},
	{L"Doc Parts",       PIDDSI_DOCPARTS},
	{MANAGERNAMES_PROP_NAME,			PIDDSI_MANAGER},
	{COMPANY_PROP_NAME,			PIDDSI_COMPANY},
	{L"Links Dirty",     PIDDSI_LINKSDIRTY},
	{0, 0}
};

pidsiStruct DocumentPropertiesService::m_pidsiArrMediaInfoSummary[] = {
	{L"Editor",        PIDMSI_EDITOR}, 
	{L"Supplier",      PIDMSI_SUPPLIER},
	{L"source",      PIDMSI_SOURCE},
	{L"Sequence Number",      PIDMSI_SEQUENCE_NO},
	{L"Project",      PIDMSI_PROJECT},
	{L"Status",       PIDMSI_STATUS},
	{L"Owner",     PIDMSI_OWNER},
	{L"Rating",      PIDMSI_RATING},
	{L"Production",	PIDMSI_PRODUCTION},
	{L"Copyright",    PIDMSI_COPYRIGHT}, 
	{0, 0}
};

void DocumentPropertiesService::populateBuiltInProperties(
							   IPropertySetStorage* aPropertySetStorage,
							   REFFMTID aFmtid,
							   DocumentPropertiesMap& aDocumentPropertiesMap )
{
	IPropertyStorage *pPropStg = NULL;
	HRESULT hr;

	// Open summary information, getting an IpropertyStorage.
	hr = aPropertySetStorage->Open(
		aFmtid,
		STGM_READ | STGM_SHARE_EXCLUSIVE, 
		&pPropStg);

	if( FAILED( hr ) ) 
	{
		return;
	}
	
	pidsiStruct* currentPidsi;

	if(aFmtid == FMTID_SummaryInformation)
	{
		currentPidsi = m_pidsiArr;
	}
	else if(aFmtid == FMTID_DocSummaryInformation)
	{
		currentPidsi = m_pidsiArrDocSummary;
	}
	else if( aFmtid == FMTID_MediaFileSummaryInformation )
	{
		currentPidsi = m_pidsiArrMediaInfoSummary;
	}
	else
	{
		// Release obtained interface.
		pPropStg->Release();

		return;
	}

	PROPSPEC spc;
	WORD wCodePage = 1255; //Default codepage 1251 (US)
	PROPVARIANT vtCodePage;

	memset( &spc, 0, sizeof(spc) );

	// Get Code page for this storage...
	spc.ulKind = PRSPEC_PROPID;
	spc.propid = PID_CODEPAGE;
	hr = pPropStg->ReadMultiple( 1, &spc, &vtCodePage );

	if (SUCCEEDED(hr) &&
		((vtCodePage.vt == VT_I4) || (vtCodePage.vt == VT_I2)))
	{
		wCodePage = LOWORD(vtCodePage.lVal);
	}
	
	PropVariantClear( &vtCodePage );

	// Count elements in pidsiArr.
	int nPidsi = 0;

	for( nPidsi=0; 
		currentPidsi[nPidsi].name; 
		nPidsi++ );

	// Initialize PROPSPEC for the properties you want.
	PROPSPEC *pPropSpec = new PROPSPEC;
	PROPVARIANT *pPropVar = new PROPVARIANT;


	for(int i=0; i < nPidsi; i++) 
	{
		ZeroMemory( pPropSpec, sizeof( PROPSPEC ) );
		pPropSpec->ulKind = PRSPEC_PROPID;
		pPropSpec->propid = currentPidsi[i].pidsi;

		hr = pPropStg->ReadMultiple( 1, pPropSpec, pPropVar );

		if ( SUCCEEDED(hr) )
		{
			addPropVariantToMap(
				pPropVar, 
				currentPidsi[i].name,
				wCodePage,
				aDocumentPropertiesMap );

			PropVariantClear( pPropVar );
		}
	}

	// De-allocate memory.
	delete pPropVar;
	delete pPropSpec;

	// Release obtained interface.
	pPropStg->Release();

}

void DocumentPropertiesService::populateCustomProperties(
							  IPropertySetStorage* aPropertySetStorage,
							  DocumentPropertiesMap& aDocumentPropertiesMap)
{
	IPropertyStorage *pPropStg = NULL;
	HRESULT hr;
	IEnumSTATPROPSTG *pEnumProp;

	// Open User-Defined-Properties, getting an IpropertyStorage.
	hr = aPropertySetStorage->Open(
		FMTID_UserDefinedProperties,
		STGM_READ | STGM_SHARE_EXCLUSIVE, 
		&pPropStg);
	
	if( FAILED( hr ) ) 
	{
		return;
	}

	// Get property enumerator.
	hr = pPropStg->Enum( &pEnumProp );

	if( FAILED( hr ) ) 
	{
		pPropStg->Release();
	    return;
	}

	PROPSPEC spc;
	WORD wCodePage = 1255; //Default codepage 1251 (US)
	PROPVARIANT vtCodePage;

	memset( &spc, 0, sizeof(spc) );

	// Get Code page for this storage...
	spc.ulKind = PRSPEC_PROPID;
	spc.propid = PID_CODEPAGE;
	hr = pPropStg->ReadMultiple( 1, &spc, &vtCodePage );

	if (SUCCEEDED(hr) &&
		((vtCodePage.vt == VT_I4) || (vtCodePage.vt == VT_I2)))
	{
		wCodePage = LOWORD(vtCodePage.lVal);
	}

	PropVariantClear( &vtCodePage );

	// Enumerate properties.
	STATPROPSTG sps;
	ULONG fetched;
	PROPSPEC propSpec[1];
	PROPVARIANT propVar[1];

	while( pEnumProp->Next( 1, &sps, &fetched ) == S_OK ) 
	{
		// Build a PROPSPEC for this property.
		ZeroMemory( &propSpec[0], sizeof( PROPSPEC ) );
		propSpec[0].ulKind = PRSPEC_PROPID;
		propSpec[0].propid = sps.propid;

		// Read this property.
		hr = pPropStg->ReadMultiple( 1, &propSpec[0], &propVar[0] );

		if( SUCCEEDED( hr ) ) 
		{
			if( NULL != sps.lpwstrName )
			{
				addPropVariantToMap(
					&propVar[0], 
					wcslwr( sps.lpwstrName ),
					wCodePage,
					aDocumentPropertiesMap );
			}

			PropVariantClear( &propVar[0] );
		}
	}

	// Release obtained interface.
	pEnumProp->Release();
	pPropStg->Release();
}

bool DocumentPropertiesService::getUnicodeXMLFromZipFile( 
	HZIP &hZIP, 
	ZIPENTRY &zipItem,  
	int zipItemIndex, 
	bstr_t& aXML )
{
	BYTE* xmlBuffer = NULL;
	xmlBuffer = new BYTE[zipItem.unc_size + 2];
	ZeroMemory( xmlBuffer, zipItem.unc_size + 2 );

	ZRESULT res = UnzipItem( hZIP, zipItemIndex, xmlBuffer, zipItem.unc_size );
	if ( ZR_OK != res )
	{
		OERROR_FILE_TRACKER1(
			"Error unzipping file" );

		delete[] xmlBuffer;

		return false;
	}

	xmlBuffer[zipItem.unc_size] = NULL;

	// convert ASCII content to unicode
	WCHAR* buffer = NULL;
	int bufferLength = -1;
	bufferLength = MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)xmlBuffer,-1, buffer, 0 );

	if( bufferLength != 0 )
	{
		buffer = new WCHAR[ bufferLength + 1 ];
		memset( buffer, 0, sizeof( WCHAR )* ( bufferLength + 1 ) );

		bufferLength = MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)xmlBuffer , -1 , buffer, bufferLength );

		aXML = buffer;
		delete []buffer; 
	}
	else
	{
		aXML = (char*)xmlBuffer;
	}

	delete [] xmlBuffer;

	return true;
}

std::wstring DocumentPropertiesService::getNodeValue( 
	MSXML2::IXMLDOMNodePtr pParentXmlNode, 
	std::wstring nodeName )
{
	MSXML2::IXMLDOMNodePtr pXmlNode = pParentXmlNode->selectSingleNode( nodeName.c_str() );
	if ( NULL == pXmlNode )
	{
		return L"";
	}

	_bstr_t nodeValue = "";

	nodeValue = pXmlNode->Gettext();

	return std::wstring( nodeValue );
}








bool	DocumentPropertiesService::isDigit(const WCHAR	wc)
{
	return (wc >= L'0' && wc <= L'9');
}

bool	DocumentPropertiesService::isDateFormat(const std::wstring &aString)
{
	const std::wstring SAMPLE_INPUT = L"1987-02-01T18:30:00Z";

	if( aString.length() != SAMPLE_INPUT.length() )
		return false;

	for( unsigned int i = 0; i < aString.length(); ++i )
	{
		bool isCurrentDigitSample = isDigit(SAMPLE_INPUT[i]);
		bool isCurrentDigitInput = isDigit(aString[i]);
		if( isCurrentDigitSample != isCurrentDigitInput )
			return false;
		if( !isCurrentDigitInput ) // if it's not a digit, the character should be the same
		{
			if( aString[i] != SAMPLE_INPUT[i] )
				return false;
		}
	}
	return true;
}

std::wstring DocumentPropertiesService::convertDate( const std::wstring &aString, bool removeLeadingZeroes, WCHAR newSeparator )
{ // this function assumes that the input is in the specified format
	std::wstring day = aString.substr(8,2);
	std::wstring month = aString.substr(5,2);
	std::wstring year = aString.substr(0,4);
	std::wstring hour = aString.substr(11,2);
	std::wstring minute = aString.substr(14,2);

	// unluckily for us, we'll need to change from system time to local time
	int day_i;
	int month_i;
	int year_i;
	int hour_i;
	int minute_i;

	swscanf_s(day.c_str(), L"%d", &day_i);
	swscanf_s(month.c_str(), L"%d", &month_i);
	swscanf_s(year.c_str(), L"%d", &year_i);
	swscanf_s(hour.c_str(), L"%d", &hour_i);
	swscanf_s(minute.c_str(), L"%d", &minute_i);

	SYSTEMTIME systemTime;
	memset(&systemTime,0,sizeof(systemTime));
	systemTime.wDay = day_i;
	systemTime.wMonth = month_i;
	systemTime.wYear = year_i;
	systemTime.wHour = hour_i;
	systemTime.wMinute = minute_i;


	SYSTEMTIME outputTime;
	SystemTimeToTzSpecificLocalTime(NULL, &systemTime, &outputTime);

	std::wstringstream	daystream;
	std::wstringstream	monthstream;
	std::wstringstream	yearstream;

	daystream << outputTime.wDay;
	monthstream << outputTime.wMonth;
	yearstream << outputTime.wYear;

	day = daystream.str();
	month = monthstream.str();
	year = yearstream.str();

	if( removeLeadingZeroes )
	{
		if( day[0] == L'0' )
			day = day.substr(1,1);

		if( month[0] == L'0' )
			month = month.substr(1,1);
	}

	return day + newSeparator + month + newSeparator + year;
}



bool DocumentPropertiesService::getPropertiesFromZipFile( 
	HANDLE hZipFile,
	DocumentPropertiesMap &propertiesMap,
	FileContainer &fileContainer )
{
	CoInitialize(NULL);

	MSXML2::IXMLDOMNodePtr pXmlNode;
	MSXML2::IXMLDOMElementPtr pEelemnt;
	MSXML2::IXMLDOMDocumentPtr xmlDOMObject;

	HRESULT hr = S_FALSE;
	hr = xmlDOMObject.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if( S_OK != hr )
	{
		OERROR_DLP_LOGIC1( "Unable to create XML parser" );
		return false;
	}

	// Open ZIp file
	HZIP hZIP = OpenZipHandle( hZipFile, NULL );

	if( NULL == hZIP )
	{
		//Not a ZIP file
		return false;
	}

	ZIPENTRY ze; 
	GetZipItem( hZIP,-1, &ze ); 
	int numOfItems = ze.index;

	ZRESULT res = -1;
	for ( int zi=0; zi < numOfItems; zi++ )
	{ 
		fileContainer.getFileSharingService().getFileSharingObject()->ResetTimer(fileContainer.getMountIndex());
		GetZipItem( hZIP, zi, &ze );

		if ( 0 == wcscmp( ze.name, L"docProps/app.xml") )
		{
			_bstr_t appXml;

			if ( !getUnicodeXMLFromZipFile( hZIP, ze, zi, appXml ) )
			{
				OERROR_DLP_LOGIC1( "Unable to extract docProps/app.xml" );
				continue;
			}

			if ( FALSE == xmlDOMObject->loadXML( appXml ) ) 
			{
				long errCode = xmlDOMObject->GetparseError()->GeterrorCode();
				_bstr_t errReason = xmlDOMObject->GetparseError()->Getreason();

				std::wstring errorSt( errReason.GetBSTR() );

				OERROR_FILE_TRACKER3(
					"Error loading docProps/app.xml XML into DOM, error Code %d, error Reason %s", errCode, errorSt.c_str() );

				return false;
			}

			pXmlNode = xmlDOMObject->selectSingleNode( L"Properties" );
			propertiesMap[APPLICATION_PROP_NAME].push_back( getNodeValue( pXmlNode, L"Application" ) );
			propertiesMap[PRESENTATIONFORMAT_PROP_NAME].push_back( getNodeValue( pXmlNode, L"PresentationFormat" ) );
			propertiesMap[SLIDES_PROP_NAME].push_back( getNodeValue( pXmlNode, L"Slides" ) );
			propertiesMap[MANAGERNAMES_PROP_NAME].push_back( getNodeValue( pXmlNode, L"Manager" ) );
			propertiesMap[COMPANY_PROP_NAME].push_back( getNodeValue( pXmlNode, L"Company" ) );
			propertiesMap[HYPERLINKBASE_PROP_NAME].push_back( getNodeValue( pXmlNode, L"HyperlinkBase" ) );
			propertiesMap[SECURITY_PROP_NAME].push_back( getNodeValue( pXmlNode, L"Security" ) );
			propertiesMap[TEMPLATES_PROP_NAME].push_back( getNodeValue( pXmlNode, L"Template" ) );
		}
		else if ( 0 == wcscmp( ze.name, L"docProps/core.xml") )
		{
			_bstr_t coreXml;

			if ( !getUnicodeXMLFromZipFile( hZIP, ze, zi, coreXml ) )
			{
				OERROR_DLP_LOGIC1( "Unable to extract docProps/core.xml" );
				continue;
			}

			if ( FALSE == xmlDOMObject->loadXML( coreXml ) ) 
			{
				long errCode = xmlDOMObject->GetparseError()->GeterrorCode();
				_bstr_t errReason = xmlDOMObject->GetparseError()->Getreason();

				std::wstring errorSt( errReason.GetBSTR() );

				OERROR_FILE_TRACKER3(
					"Error loading docProps/core.xml XML into DOM, error Code %d, error Reason %s", errCode, errorSt.c_str() );

				return false;
			}

			pXmlNode = xmlDOMObject->selectSingleNode( L"cp:coreProperties" );
			propertiesMap[AUTHORS_PROP_NAME].push_back( getNodeValue( pXmlNode, L"dc:creator" ) );
			propertiesMap[TITLES_PROP_NAME].push_back( getNodeValue( pXmlNode, L"dc:title" ) );
			propertiesMap[SUBJECTS_PROP_NAME].push_back( getNodeValue( pXmlNode, L"dc:subject" ) );
			propertiesMap[KEYWORDS_PROP_NAME].push_back( getNodeValue( pXmlNode, L"cp:keywords" ) );
			propertiesMap[COMMENTS_PROP_NAME].push_back( getNodeValue( pXmlNode, L"dc:description" ) );
			propertiesMap[CATEGORIES_PROP_NAME].push_back( getNodeValue( pXmlNode, L"cp:category" ) );
			propertiesMap[REVISION_PROP_NAME].push_back( getNodeValue( pXmlNode, L"cp:revision" ) );
			propertiesMap[LASTSAVEDBY_PROP_NAME].push_back( getNodeValue( pXmlNode, L"cp:lastModifiedBy" ) );
			propertiesMap[STATUS_PROP_NAME].push_back( getNodeValue( pXmlNode, L"cp:contentStatus" ) );
		}
		else if ( 0 == wcscmp( ze.name, L"docProps/custom.xml") )
		{
			_bstr_t customXml;

			if ( !getUnicodeXMLFromZipFile( hZIP, ze, zi, customXml ) )
			{
				OERROR_DLP_LOGIC1( "Unable to extract docProps/custom.xml" );
				continue;
			}

			if ( FALSE == xmlDOMObject->loadXML( customXml ) ) 
			{
				long errCode = xmlDOMObject->GetparseError()->GeterrorCode();
				_bstr_t errReason = xmlDOMObject->GetparseError()->Getreason();
				
				std::wstring errorSt( errReason.GetBSTR() );

				OERROR_FILE_TRACKER3(
					"Error loading docProps/custom.xml XML into DOM, error Code %d, error Reason %s", errCode, errorSt.c_str() );

				return false;
			}

			// Get root of all properties
			pXmlNode = xmlDOMObject->selectSingleNode( L"Properties" );

			MSXML2::IXMLDOMNodeListPtr pNodeList =  pXmlNode->selectNodes(L"property");

			long numberOfSubNodes = 0;
			pNodeList->get_length(&numberOfSubNodes);

			// Iterate all properties
			for ( int i = 0; i < numberOfSubNodes; i++ )
			{
				hr = pNodeList->get_item( i, &pXmlNode );
				if ( !SUCCEEDED(hr) )
				{
					continue;
				} 

				// Get all attributes of a property
				MSXML2::IXMLDOMNamedNodeMapPtr nodeAttributes;
				hr = pXmlNode->get_attributes( &nodeAttributes );
				if ( !SUCCEEDED( hr) )
				{
					continue;
				}
				 
				MSXML2::IXMLDOMNodePtr pXmlNodeAttribute = nodeAttributes->getNamedItem( L"fmtid" );
				if( NULL == pXmlNodeAttribute )
				{
					continue;
				}

				// Get the source property (and then continue with the custom ones..)
				std::wstring attributeValue = pXmlNodeAttribute->Gettext();
				if ( 0 == attributeValue.compare( L"{64440492-4C8B-11D1-8B70-080036B11A03}" ) )
				{
					propertiesMap[SOURCE_PROP_NAME].push_back( getNodeValue( pXmlNode, L"vt:lpwstr" ) );
				}
				else
				{
					pXmlNodeAttribute = nodeAttributes->getNamedItem( L"name" );
					if( NULL == pXmlNodeAttribute )
					{
						continue;
					}

					attributeValue = pXmlNodeAttribute->Gettext();	// Custom property name
					if ( 0 != attributeValue.compare( L"" ) )
					{
						MSXML2::IXMLDOMNodeList	*pSubNodes;
						hr = pXmlNodeAttribute->get_childNodes(&pSubNodes);
						if( SUCCEEDED(hr) && pSubNodes )
						{
							long num_childes = 0;
							pSubNodes->get_length(&num_childes);
							for( int child_index = 0; child_index < num_childes; ++child_index )
							{
								MSXML2::IXMLDOMNodePtr current = pSubNodes->Getitem(child_index);

								_bstr_t nodeValue = "";
								
								nodeValue = pXmlNode->Gettext();

								std::wstring nodeValueStr(nodeValue);

								if( nodeValueStr == L"true")
									nodeValueStr = L"Yes";
								else if( nodeValueStr == L"false" )
									nodeValueStr = L"No";
								if( isDateFormat(nodeValueStr) )
									nodeValueStr = convertDate(nodeValueStr);

								propertiesMap[attributeValue].push_back( nodeValueStr );
							}

							pSubNodes->Release();
						}
					}
				}
			}
		}
	}

	res = CloseZip( hZIP );
	//_ASSERT( res == ZR_OK );

	return true;
}

BOOL	DocumentPropertiesService::extractFileInformation(
	FileContainer	&fileContainer,
	UINT32& aCreationDate,
	UINT32& aModificationDate,
	UINT32& aFileSize )
{
	BY_HANDLE_FILE_INFORMATION fileInfo;

	::memset( &fileInfo, 0, sizeof(BY_HANDLE_FILE_INFORMATION) );

	if ( FALSE != ::GetFileInformationByHandle( fileContainer.getFileHandle(), &fileInfo ) )
	{
		//Set creation time
		ACE_Time_Value tvCreationTime( fileInfo.ftCreationTime );
		timespec_t tsCreationTime = tvCreationTime;
		aCreationDate = (UINT32)tsCreationTime.tv_sec;

		//Set modification time
		ACE_Time_Value tvModificationTime( fileInfo.ftLastWriteTime );
		timespec_t tsModificationTime = tvModificationTime;
		aModificationDate = (UINT32)tsModificationTime.tv_sec;

		if( fileInfo.nFileSizeHigh )
		{
			//For now files larger then 2^32 (UINT32 is not enough) are not supported,
			//so limit the size to the maximum value UINT32 can hold minus one
			aFileSize = FILE_SIZE_CEILING_VALUE;
		}
		else
		{
			aFileSize = fileInfo.nFileSizeLow;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL	DocumentPropertiesService::extractFileInformation(
	const std::wstring& filePath,
	const std::wstring& logicalFilePath,
	UINT32 aMountIndex, 
	UINT32 aSafebootMountIndex, 
	BOOL aIsLocalDrive,
	UINT32& aCreationDate,
	UINT32& aModificationDate,
	UINT32& aFileSize )
{
	FileContainer fileContainer( 
		filePath,
		*mFileSharingService,
		aMountIndex,
		aSafebootMountIndex,
		aIsLocalDrive,
		FALSE,
		GENERIC_READ );

	if( 0 != fileContainer.openFile() )
	{
		return FALSE;
	}

	BOOL result = extractFileInformation(fileContainer, aCreationDate, aModificationDate, aFileSize);
	fileContainer.close();
	return result;
}


bool DocumentPropertiesService::getDocumentPropertiesFromFile( 
	std::wstring& filePath, 
	std::wstring& logicalFilePath, 
	UINT32 aMountIndex, 
	UINT32 aSafebootMountIndex, 
	BOOL aIsLocalDrive,
	UINT32 aMaxFileSizeForExtraction,
	DocumentPropertiesMap& aDocumentPropertiesMap,
	UINT32& aCreationDate,
	UINT32& aModificationDate,
	UINT32& aFileSize )
{
	FileContainer fileContainer( 
		filePath,
		*mFileSharingService,
		aMountIndex,
		aSafebootMountIndex,
		aIsLocalDrive,
		FALSE,
		GENERIC_READ );

	if( 0 != fileContainer.openFile() )
	{
		return FALSE;
	}

	if( !extractFileInformation(fileContainer, aCreationDate, aModificationDate, aFileSize) )
	{
		return FALSE;
	}

	if( aFileSize > aMaxFileSizeForExtraction )
	{
		return FALSE;
	}

	/// Handle DocProps in Compound files (office 2003 and below)
	IPropertySetStorage *pPropSetStg = NULL;
	HRESULT hr = S_OK;
	DWORD dwOpenMode = (STGM_READ | STGM_SHARE_EXCLUSIVE);
	IPropertyStorage *pPropStg = NULL;
	IStorage* iStorage;
	
	CILockBytesOnFileHandle* lockBytes = NULL;
	lockBytes = new CILockBytesOnFileHandle;
	lockBytes->HrInit( (HANDLE)fileContainer.getFileHandle() );

	IStorage *pistgInput = NULL;
	hr = StgIsStorageILockBytes( lockBytes );
	
	if ( S_OK == hr )
	{
		hr = StgOpenStorageOnILockBytes(
			lockBytes, 
			NULL, 
			( STGM_READ | STGM_TRANSACTED | STGM_SHARE_DENY_NONE ), 
			NULL, 
			NULL, 
			&iStorage );

		if( SUCCEEDED( hr ) )
		{
			hr = iStorage->QueryInterface( IID_IPropertySetStorage, (void**)&pPropSetStg );

			if( !SUCCEEDED( hr ) || NULL == pPropSetStg )
			{
				ODEBUG_FILE_TRACKER2(
					"Unable to get interface IID_IPropertySetStorage fordocument  for %s" ,
					filePath.c_str() );

				iStorage->Release();
				delete lockBytes;
				fileContainer.close();
				return false;
			}

			hr = pPropSetStg->Open( 
				FMTID_SummaryInformation, 
				dwOpenMode,
				&pPropStg );


			if( SUCCEEDED( hr ) )
			{
				pPropStg->Release();

				populateBuiltInProperties( 
					pPropSetStg, 
					FMTID_SummaryInformation, 
					aDocumentPropertiesMap );

				populateBuiltInProperties( 
					pPropSetStg, 
					FMTID_DocSummaryInformation,
					aDocumentPropertiesMap );

				populateBuiltInProperties( 
					pPropSetStg, 
					FMTID_MediaFileSummaryInformation,
					aDocumentPropertiesMap );

				populateCustomProperties( 
					pPropSetStg, 
					aDocumentPropertiesMap );

				pPropSetStg->Release();
				pPropSetStg = NULL;
			
			}

			iStorage->Release();
		}
		
		delete lockBytes;
		fileContainer.close();
		return true;
	}
	
	delete lockBytes;

	// Handle DocProps in office 2007 files
	if ( getPropertiesFromZipFile( 
		(HANDLE)fileContainer.getFileHandle(),
		aDocumentPropertiesMap,
		fileContainer ) )
	{
		fileContainer.close();
		return true;
	}

	// Handle properties in ADT
	// TODO: need to implement
	
	fileContainer.close();
	return false;
}

HKEY DocumentPropertiesService::GetHKCRKey(
	WCHAR* pszKeyString, 
	WCHAR* pszPart)
{
	HKEY hk = NULL;
	WCHAR szRegKey[(MAX_PATH * 2)]; 
	wsprintf( szRegKey, pszKeyString, pszPart );
	RegOpenKey( HKEY_CLASSES_ROOT, szRegKey, &hk );
	return hk;
}

void DocumentPropertiesService::addPropVariantToMap(
	PROPVARIANT* pPropVar,
	WCHAR* aName,
	WORD aCodePage,
	DocumentPropertiesMap& aDocumentPropertiesMap ) 
{
	if(pPropVar->vt & VT_ARRAY) {
		return;
	}

	if(pPropVar->vt & VT_BYREF) {
		return;
	}

	// Switch types.
	switch(pPropVar->vt) {
	  case VT_EMPTY:
		  break;
	  case VT_NULL:
		  break;
	  case VT_BLOB:
		  break;
	  case VT_BOOL:
		  {
			  std::wstring bvalue= pPropVar->boolVal ? L"TRUE" : L"FALSE";
			  aDocumentPropertiesMap[aName].push_back( bvalue );
		  }
		  break;
	  case VT_I2: // 2-byte signed int.
		  aDocumentPropertiesMap[aName].push_back( toString( (int)pPropVar->iVal ) );
		  break;
	  case VT_I4: // 4-byte signed int.
		  aDocumentPropertiesMap[aName].push_back( toString( (int)pPropVar->lVal ) );
		  break;
	  case VT_R4: // 4-byte real.
		  aDocumentPropertiesMap[aName].push_back( toString( (double)pPropVar->fltVal ) );
		  break;
	  case VT_R8: // 8-byte real.
		  aDocumentPropertiesMap[aName].push_back( toString( (double)pPropVar->dblVal ) );
		  break;
	  case VT_BSTR: // OLE Automation string.
		  aDocumentPropertiesMap[aName].push_back( pPropVar->bstrVal );
		  break;
	  case VT_LPSTR: // Null-terminated string.
		  {
			  // convert ascii content to unicode
			  WCHAR* buffer = NULL;
			  int bufferLength;
		
			  bufferLength = MultiByteToWideChar( aCodePage, 0, pPropVar->pszVal,-1, buffer, 0 );

			  if( bufferLength != 0 )
			  {
				  buffer = new WCHAR[ bufferLength + 1 ];

				  memset( buffer, 0, sizeof( WCHAR )* ( bufferLength + 1 ) );

				  int code1 = ::GetACP();
				  bufferLength = MultiByteToWideChar( aCodePage, 0, pPropVar->pszVal , -1 , buffer, bufferLength );

				  if( bufferLength != 0 )
				  {
					  aDocumentPropertiesMap[aName].push_back( buffer );
				  }
				  delete[] buffer; 
			  }
		  }
		  break;
	  case VT_LPWSTR:
		  aDocumentPropertiesMap[aName].push_back( pPropVar->pwszVal );
		  break;
	  case VT_FILETIME:
		  {
			  FILETIME lft;
			  FileTimeToLocalFileTime(&pPropVar->filetime, &lft); 
			  UINT64 fTime = 0;
			  fTime |= lft.dwHighDateTime;
			  fTime <<= 32;
			  fTime |= lft.dwLowDateTime;
			  aDocumentPropertiesMap[aName].push_back( toString( fTime ) );
		  }
		  break;
	  case VT_DATE:
		  {
			  SYSTEMTIME sysTime; 
			  VariantTimeToSystemTime(pPropVar->date, &sysTime);                            
			  FILETIME lft;
			  SystemTimeToFileTime(&sysTime, &lft);
			  UINT64 fTime = 0;
			  fTime |= lft.dwHighDateTime;
			  fTime <<= 32;
			  fTime |= lft.dwLowDateTime;
			  aDocumentPropertiesMap[aName].push_back( toString( fTime ) );
		  }
		  break;
	  case VT_CF: // Clipboard format.
		  break;
	  default: // Unhandled type, consult wtypes.h's VARENUM structure.
		  break;
	}
}


std::wstring DocumentPropertiesService::getFileOwner( AutoFileInfo aFileInfo )
{
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	DWORD accountNameSize = 0;
	DWORD domainNameSize = 0;
	SID_NAME_USE eUse = SidTypeUnknown;
	PSECURITY_DESCRIPTOR pSD = NULL;

	FileContainer fileContainer( 
		aFileInfo->mFullFileName,
		*mFileSharingService,
		aFileInfo->mMountIndex,
		aFileInfo->mSafebootMountIndex,
		EV_FS_TYPE_NETWORK != aFileInfo->mFsType,
		FALSE,
		GENERIC_READ );

	if( 0 != fileContainer.openFile() )
	{
		return L"";
	}

	return FileSystemUtils::getFileOwner(fileContainer.getFileHandle());
}

bool DocumentPropertiesService::getXMLFromZipFile( HZIP &hZIP, ZIPENTRY &zipItem, int zipItemIndex, _bstr_t& aXML )
{
	char* xmlBuffer = NULL;
	xmlBuffer = new char[zipItem.unc_size + 1];

	ZRESULT res = UnzipItem( hZIP, zipItemIndex, xmlBuffer, zipItem.unc_size );
	if( ZR_OK != res )
	{
		delete[] xmlBuffer;

		return false;
	}

	xmlBuffer[zipItem.unc_size] = NULL;

	aXML = xmlBuffer;

	delete [] xmlBuffer;

	return true;
}

bool DocumentPropertiesService::addContentTypesCustomPropsInfo( HZIP hZIP, StringPairList& aCustomXmls )
{
	ZIPENTRY ze; 
	int index;
	ZRESULT zr = FindZipItem( hZIP, L"[Content_Types].xml", true, &index, &ze );
	if( zr != ZR_OK )
	{
		return false;
	}

	_bstr_t contentTypesXml;

	if ( !getXMLFromZipFile( hZIP, ze, index, contentTypesXml ) )
	{
		return false;
	}

	MSXML2::IXMLDOMDocumentPtr xmlDOMObject;

	if( xmlDOMObject.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK )
	{
		return false;
	}

	if ( false == xmlDOMObject->loadXML( contentTypesXml ) ) 
	{
		return false;
	}

	MSXML2::IXMLDOMNodePtr typesNode = xmlDOMObject->selectSingleNode( L"Types" );

	_bstr_t namespaceURI = typesNode->GetnamespaceURI();

	MSXML2::IXMLDOMElementPtr newOverride = xmlDOMObject->createNode( NODE_ELEMENT, L"Override", namespaceURI );

	MSXML2::IXMLDOMAttributePtr partNameAttribute = xmlDOMObject->createAttribute( L"PartName" );
	partNameAttribute->value = L"/docProps/custom.xml";

	MSXML2::IXMLDOMAttributePtr contentTypeAttribute = xmlDOMObject->createAttribute( L"ContentType" );
	contentTypeAttribute->value = L"application/vnd.openxmlformats-officedocument.custom-properties+xml";

	newOverride->setAttributeNode( partNameAttribute );
	newOverride->setAttributeNode( contentTypeAttribute );

	typesNode->appendChild( newOverride );

	aCustomXmls.push_back(make_pair(L"[Content_Types].xml", xmlDOMObject->Getxml()));

	return true;
}

bool DocumentPropertiesService::addRelsCustomPropsInfo( HZIP hZIP, StringPairList& aCustomXmls )
{
	ZIPENTRY ze; 
	int index;
	ZRESULT zr = FindZipItem( hZIP, L"_rels/.rels", true, &index, &ze );
	if( zr != ZR_OK )
	{
		return false;
	}

	_bstr_t relsXml;

	if ( !getXMLFromZipFile( hZIP, ze, index, relsXml ) )
	{
		return false;
	}

	MSXML2::IXMLDOMDocumentPtr xmlDOMObject;

	if( xmlDOMObject.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK )
	{
		return false;
	}

	if ( false == xmlDOMObject->loadXML( relsXml ) ) 
	{
		return false;
	}

	MSXML2::IXMLDOMNodePtr relationshipsNode = xmlDOMObject->selectSingleNode( L"Relationships" );

	int rid;
	if( !getHighestID( relationshipsNode, L"Relationship", L"Id", L"rId", rid ) )
	{
		return false;
	}

	_bstr_t namespaceURI = relationshipsNode->GetnamespaceURI();

	MSXML2::IXMLDOMElementPtr newRelationship = xmlDOMObject->createNode( NODE_ELEMENT, L"Relationship", namespaceURI );

	MSXML2::IXMLDOMAttributePtr idAttribute = xmlDOMObject->createAttribute( L"Id" );
	idAttribute->value = ( L"rId" + toString(rid) ).c_str();

	MSXML2::IXMLDOMAttributePtr typeAttribute = xmlDOMObject->createAttribute( L"Type" );
	typeAttribute->value = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/custom-properties";

	MSXML2::IXMLDOMAttributePtr targetAttribute = xmlDOMObject->createAttribute( L"Target" );
	targetAttribute->value = L"docProps/custom.xml";

	newRelationship->setAttributeNode( idAttribute );
	newRelationship->setAttributeNode( typeAttribute );
	newRelationship->setAttributeNode( targetAttribute );

	relationshipsNode->appendChild( newRelationship );

	aCustomXmls.push_back(make_pair(L"_rels/.rels", xmlDOMObject->Getxml()) );

	return true;
}

MSXML2::IXMLDOMElementPtr DocumentPropertiesService::createCustomPropertyNode( 
	MSXML2::IXMLDOMDocumentPtr aXmlDOMObject, 
	const wchar_t* aNamespaceURI,
	const std::wstring& aName, 
	const std::wstring& aValue,
	int aPid )
{
	MSXML2::IXMLDOMElementPtr newProperty = aXmlDOMObject->createNode( NODE_ELEMENT, L"property", aNamespaceURI );

	MSXML2::IXMLDOMAttributePtr fmtidAttribute = aXmlDOMObject->createAttribute( L"fmtid" );
	fmtidAttribute->value = L"{D5CDD505-2E9C-101B-9397-08002B2CF9AE}";

	MSXML2::IXMLDOMAttributePtr pidAttribute = aXmlDOMObject->createAttribute( L"pid" );
	pidAttribute->value = aPid;

	MSXML2::IXMLDOMAttributePtr xmlnsAttribute = aXmlDOMObject->createAttribute( L"xmlns:vt" );
	xmlnsAttribute->value = L"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes";

	MSXML2::IXMLDOMAttributePtr nameAttribute = aXmlDOMObject->createAttribute( L"name" );
	nameAttribute->value = aName.c_str();

	newProperty->setAttributeNode( fmtidAttribute );
	newProperty->setAttributeNode( pidAttribute );
	newProperty->setAttributeNode( xmlnsAttribute );
	newProperty->setAttributeNode( nameAttribute );

	MSXML2::IXMLDOMElementPtr textValue = aXmlDOMObject->createElement( L"vt:lpwstr" );
	MSXML2::IXMLDOMTextPtr tagID = aXmlDOMObject->createTextNode( aValue.c_str() );
	textValue->appendChild( tagID );
	newProperty->appendChild( textValue );

	return newProperty;
}

void DocumentPropertiesService::updateCustomPropertyNode( 
	MSXML2::IXMLDOMDocumentPtr aXmlDOMObject, 
	MSXML2::IXMLDOMNodePtr aPropertiesNode,
	const std::wstring& aName, 
	const std::wstring& aValue,
	int& aPid )
{
	if( aValue == IGNORE_PROPERTY_VALUE )
	{
		return;
	}

	MSXML2::IXMLDOMNodePtr propertyNode = aPropertiesNode->selectSingleNode( (L"property[@name=\"" + aName + L"\"]").c_str() );

	if( propertyNode )
	{
		if( aValue.empty() || !propertyNode->selectSingleNode( L"vt:lpwstr" ) )
		{
			aPropertiesNode->removeChild( propertyNode );
			propertyNode = NULL;
		}
	}

	if( !aValue.empty() )
	{
		if( propertyNode )
		{
			propertyNode->selectSingleNode( L"vt:lpwstr" )->Puttext( aValue.c_str() );
		}
		else
		{
			MSXML2::IXMLDOMElementPtr newProperty = createCustomPropertyNode( aXmlDOMObject, aPropertiesNode->GetnamespaceURI(), aName, aValue, aPid );
			aPropertiesNode->appendChild( newProperty );
			++aPid;
		}
	}
}

bool DocumentPropertiesService::getHighestID( 
	MSXML2::IXMLDOMNodePtr aPropertiesNode, 
	LPCWSTR aNodeName, 
	LPCWSTR aNodeIDAttribute, 
	LPCWSTR aIDPrefix,
	int& aHighestID )
{
	MSXML2::IXMLDOMNodeListPtr propertiesList = aPropertiesNode->selectNodes( aNodeName );

	long numberOfSubNodes = 0;
	propertiesList->get_length(&numberOfSubNodes);

	aHighestID = 0;

	MSXML2::IXMLDOMNodePtr propertyNode;

	// Iterate all properties
	for ( int i = 0; i < numberOfSubNodes; i++ )
	{
		HRESULT hr = propertiesList->get_item( i, &propertyNode );
		if ( !SUCCEEDED(hr) )
		{
			return false;
		} 

		// Get all attributes of a property
		MSXML2::IXMLDOMNamedNodeMapPtr nodeAttributes;
		hr = propertyNode->get_attributes( &nodeAttributes );
		if ( !SUCCEEDED( hr) )
		{
			return false;
		}

		MSXML2::IXMLDOMNodePtr idAttribute = nodeAttributes->getNamedItem( aNodeIDAttribute );
		if( NULL == idAttribute )
		{
			return false;
		}

		std::wstring idText = idAttribute->Gettext();
	
		size_t prefixLength = wcslen(aIDPrefix);

		if( prefixLength != 0 && idText.find( aIDPrefix ) == 0 )
		{
			idText = idText.substr( prefixLength );
		}

		int id = _wtoi( idText.c_str() );
		if( id > aHighestID )
		{
			aHighestID = id;
		}
	}

	++aHighestID;

	return true;
}

bool DocumentPropertiesService::createCustomXmls( 
	HZIP hZIP, 
	StringPairList& aCustomXmls, 
	const DLPCustomProperties& aCustomProperties )
{
	ZIPENTRY ze; 
	int index;
	ZRESULT zr = FindZipItem( hZIP, L"docProps/custom.xml", true, &index, &ze );
	if( zr != ZR_OK && zr != ZR_NOTFOUND )
	{
		return false;
	}

	_bstr_t customXml;

	if( zr == ZR_NOTFOUND )
	{
		if( !addRelsCustomPropsInfo( hZIP, aCustomXmls ) )
		{
			return false;
		}

		if( !addContentTypesCustomPropsInfo( hZIP, aCustomXmls ) )
		{
			return false;
		}

		customXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
					 <Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/custom-properties\" \
					 xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\"></Properties>";
	}
	else
	{
		if ( !getXMLFromZipFile( hZIP, ze, index, customXml ) )
		{
			return false;
		}
	}

	MSXML2::IXMLDOMDocumentPtr xmlDOMObject;

	if( xmlDOMObject.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK )
	{
		return false;
	}

	if ( false == xmlDOMObject->loadXML( customXml ) ) 
	{
		return false;
	}

	MSXML2::IXMLDOMNodePtr propertiesNode = xmlDOMObject->selectSingleNode( L"Properties" );

	int pid;
	if( !getHighestID( propertiesNode, L"property", L"pid", L"", pid ) )
	{
		return false;
	}

	if( pid == 1 )
	{
		//pid must start from 2
		++pid;
	}

	updateCustomPropertyNode( xmlDOMObject, propertiesNode, CUSTOM_PROPERTY_CLASSIFICATIONS, aCustomProperties.mClassifications, pid );
	updateCustomPropertyNode( xmlDOMObject, propertiesNode, CUSTOM_PROPERTY_CLASSIFIER, aCustomProperties.mClassifier, pid );
	updateCustomPropertyNode( xmlDOMObject, propertiesNode, CUSTOM_PROPERTY_DATE, aCustomProperties.mDate, pid );
	updateCustomPropertyNode( xmlDOMObject, propertiesNode, CUSTOM_PROPERTY_AUTO_CLASSIFICATIONS, aCustomProperties.mAutoClassifications, pid );
	updateCustomPropertyNode( xmlDOMObject, propertiesNode, CUSTOM_PROPERTY_MANUAL_VERSION, aCustomProperties.mManualVersion, pid );
	updateCustomPropertyNode( xmlDOMObject, propertiesNode, CUSTOM_PROPERTY_AUTO_VERSION, aCustomProperties.mAutoVersion, pid );

	aCustomXmls.push_back(std::make_pair( L"docProps/custom.xml", xmlDOMObject->Getxml()) );

	return true;
}


//McAfee: Modified example of modify.cpp example from ZipUtils - Fixed bugs in the example and added buffer arguments (to add a file to zip from memory buffer).
//if the buffer arguments are used zefn should be NULL.

// AddFileToZip: adds a file to a zip, possibly replacing what was there before
// zipfn ="c:\\archive.zip"             (the fn of the zip file) 
// zefn  ="c:\\my documents\\file.txt"  (the fn of the file to be added)
// zename="file.txt"                    (the name that zefn will take inside the zip)
// If zefn is empty, we just delete zename from the zip archive.
// The way it works is that we create a temporary zipfile, and copy the original
// contents into the new one (with the appropriate addition or substitution)
// and then remove the old one and rename the new one. NB. we are case-insensitive.
ZRESULT DocumentPropertiesService::AddFileToZip(const TCHAR *zipfn, const TCHAR *zename, const TCHAR *zefn, char* inBuf, int inBufLen)
{ 
	if( GetFileAttributes(zipfn) == INVALID_FILE_ATTRIBUTES || ( zefn != 0 && GetFileAttributes(zefn) == INVALID_FILE_ATTRIBUTES) )
	{
		return ZR_NOFILE;
	}

	// Expected size of the new zip will be the size of the old zip plus the size of the new file
	HANDLE hf = CreateFile(zipfn,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0); 
	if (hf==INVALID_HANDLE_VALUE) 
	{
		return ZR_NOFILE; 
	}

	DWORD size=GetFileSize(hf,0); 
	CloseHandle(hf);

	if (zefn!=0) 
	{
		hf=CreateFile(zefn,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0); 
		if (hf==INVALID_HANDLE_VALUE) 
		{
			return ZR_NOFILE; 
		}
		size+=GetFileSize(hf,0); 
		CloseHandle(hf);
	}
	else
	{
		size+=inBufLen;
	}

	//size*=2; // just to be on the safe side.

	HZIP hzsrc=OpenZip(zipfn,0); 
	if (hzsrc==0) 
	{
		return ZR_READ;
	}
	HZIP hzdst=CreateZip(0,size,0); 
	if (hzdst==0) 
	{
		CloseZip(hzsrc); 
		return ZR_WRITE;
	}
	// hzdst is created in the system pagefile
	// Now go through the old zip, unzipping each item into a memory buffer, and adding it to the new one
	char *buf=0; unsigned int bufsize=0; // we'll unzip each item into this memory buffer
	ZIPENTRY ze; 
	ZRESULT zr=GetZipItem(hzsrc,-1,&ze); 
	int numitems=ze.index; 
	if (zr!=ZR_OK) 
	{
		CloseZip(hzsrc); 
		CloseZip(hzdst); 
		return zr;
	}

	for (int i=0; i<numitems; i++)
	{ 
		zr=GetZipItem(hzsrc,i,&ze); 
		if (zr!=ZR_OK) 
		{
			CloseZip(hzsrc); 
			CloseZip(hzdst); 
			return zr;
		}
		if (wcscmp(ze.name,zename)==0) 
		{
			continue; // don't copy over the old version of the file we're changing
		}
		if (ze.attr&FILE_ATTRIBUTE_DIRECTORY) 
		{
			zr=ZipAddFolder(hzdst,ze.name); 
			if (zr!=ZR_OK) 
			{
				CloseZip(hzsrc); 
				CloseZip(hzdst); 
				return zr;
			} 
			continue;
		}
		if (ze.unc_size>(long)bufsize) 
		{
			if (buf!=0) 
			{ 
				delete[] buf;
			}
			bufsize=ze.unc_size; 
			buf=new char[bufsize];
		}
		zr=UnzipItem(hzsrc,i,buf,ze.unc_size); 
		if (zr!=ZR_OK) 
		{
			CloseZip(hzsrc); 
			CloseZip(hzdst); 
			return zr;
		}
		zr=ZipAdd(hzdst,ze.name,buf,ze.unc_size); 
		if (zr!=ZR_OK) 
		{
			CloseZip(hzsrc); 
			CloseZip(hzdst); 
			return zr;
		}
	}

	delete[] buf;
	// Now add the new file
	if (zefn!=0) 
	{
		zr=ZipAdd(hzdst,zename,zefn);
	}
	else
	{
		zr=ZipAdd(hzdst,zename,inBuf, inBufLen); 
	}

	if (zr!=ZR_OK) 
	{
		CloseZip(hzsrc); 
		CloseZip(hzdst); 
		return zr;
	}

	zr=CloseZip(hzsrc); 
	if (zr!=ZR_OK) 
	{
		CloseZip(hzdst); 
		return zr;
	}
	//
	// The new file has been put into pagefile memory. Let's store it to disk, overwriting the original zip
	zr=ZipGetMemory(hzdst,(void**)&buf,&size); 
	if (zr!=ZR_OK) 
	{
		CloseZip(hzdst); 
		return zr;
	}
	hf=CreateFile(zipfn,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); 
	if (hf==INVALID_HANDLE_VALUE) 
	{
		CloseZip(hzdst); 
		return ZR_WRITE;
	}
	DWORD writ; 
	WriteFile(hf,buf,size,&writ,0); 
	CloseHandle(hf);
	zr=CloseZip(hzdst); 
	if (zr!=ZR_OK) 
	{
		return zr;
	}

	return ZR_OK;
}

bool DocumentPropertiesService::openFileExclusively( 
	const std::wstring& aFilePath, 
	ManualClassificationError& aError )
{
	HANDLE hSourceFile = CreateFile( aFilePath.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );

	if( hSourceFile == INVALID_HANDLE_VALUE )
	{
		aError = GetLastError() == ERROR_SHARING_VIOLATION ? MT_ERROR_FILE_LOCKED : MT_ERROR_ACCESS_FAILED;
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to open file: %s. Error %d", aFilePath.c_str(), GetLastError() );
		return false;
	}

	CloseHandle( hSourceFile );

	return true;
}

bool DocumentPropertiesService::copyFileToTemp( 
	const std::wstring& aFilePath,
	ManualClassificationError& aError, 
	std::wstring& aTempFileName )
{
	WCHAR tempFileName[MAX_PATH];
	if( !GetTempFileName( mAgentTempFolder.c_str(), L"DPS", 0, tempFileName ) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to create temp file for: %s. Error %d", aFilePath.c_str(), GetLastError() );
		return false;
	}

	if( !CopyFile( aFilePath.c_str(), tempFileName, FALSE ) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to copy file to temp: %s. Error %d", aFilePath.c_str(), GetLastError() );
		DeleteFile( tempFileName );
		return false;
	}

	aTempFileName = tempFileName;

	return true;
}

void DocumentPropertiesService::addPropertyToWrite( 
	PROPSPEC* aPropsec, 
	PROPVARIANT* aPropvarWrite, 
	int& aIdx, 
	LPWSTR aName, 
	const std::wstring& aValue )
{
	if( !aValue.empty() )
	{
		aPropsec[aIdx].ulKind = PRSPEC_LPWSTR;
		aPropsec[aIdx].lpwstr = aName;
		aPropvarWrite[aIdx].vt = VT_LPWSTR;
		aPropvarWrite[aIdx].pwszVal = (LPWSTR)aValue.c_str();
		++aIdx;
	}
}

bool DocumentPropertiesService::setDlpStorageCustomProperties( 
	const std::wstring& aFilePath,
	const DLPCustomProperties& aCustomProperties )
{
	IPropertySetStorage *pPropSetStg = NULL;
	IPropertyStorage *pPropStg = NULL;

	HRESULT hr = StgOpenStorageEx( aFilePath.c_str(), STGM_READWRITE|STGM_SHARE_EXCLUSIVE, STGFMT_DOCFILE, 0, NULL, NULL, IID_IPropertySetStorage, (void**)&pPropSetStg );

	if( FAILED(hr) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"StgOpenStorageEx failed: %d", hr );
		return false;
	}

	hr = pPropSetStg->Open(FMTID_UserDefinedProperties, STGM_READWRITE|STGM_SHARE_EXCLUSIVE, &pPropStg);
	if( hr == STG_E_FILENOTFOUND )
	{
		hr = pPropSetStg->Create(FMTID_UserDefinedProperties, NULL, PROPSETFLAG_DEFAULT, STGM_READWRITE|STGM_SHARE_EXCLUSIVE, &pPropStg);
	}

	if( FAILED(hr) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed opening user properties: %d", hr );
		pPropSetStg->Release();
		return false;
	}

	PROPSPEC propspec[6]; 
	PROPVARIANT propvarWrite[6]; 
	int idx = 0;

	addPropertyToWrite( propspec, propvarWrite, idx, CUSTOM_PROPERTY_CLASSIFICATIONS, aCustomProperties.mClassifications );
	addPropertyToWrite( propspec, propvarWrite, idx, CUSTOM_PROPERTY_CLASSIFIER, aCustomProperties.mClassifier );
	addPropertyToWrite( propspec, propvarWrite, idx, CUSTOM_PROPERTY_DATE, aCustomProperties.mDate );
	addPropertyToWrite( propspec, propvarWrite, idx, CUSTOM_PROPERTY_AUTO_CLASSIFICATIONS, aCustomProperties.mAutoClassifications );
	addPropertyToWrite( propspec, propvarWrite, idx, CUSTOM_PROPERTY_MANUAL_VERSION, aCustomProperties.mManualVersion );
	addPropertyToWrite( propspec, propvarWrite, idx, CUSTOM_PROPERTY_AUTO_VERSION, aCustomProperties.mAutoVersion );

	hr = pPropStg->WriteMultiple( idx, propspec, propvarWrite, PID_FIRST_USABLE );

	if( FAILED(hr) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed writing properties: %d", hr );
		pPropStg->Release();
		pPropSetStg->Release();
		return false;
	}

	hr = pPropStg->Commit(STGC_DEFAULT);

	if( FAILED(hr) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Commit failed: %d", hr );
		pPropStg->Release();
		pPropSetStg->Release();

		return false;
	}

	pPropStg->Release();
	pPropSetStg->Release();

	return true;
}

bool DocumentPropertiesService::setDlpCustomProperties( 
	const std::wstring& aFilePath,
	const DLPCustomProperties& aCustomProperties,
	ManualClassificationError& aError )
{
	aError = MT_ERROR_INTERNAL;

	if( aCustomProperties.mClassifications.length() > 200 ||
		aCustomProperties.mAutoClassifications.length() > 200 ||
		aCustomProperties.mClassifier.length() > 200 )
	{
		aError = MT_ERROR_MAX_PROPERTY_LENGTH;
		OLOG( OLoggingManager::OSEV_ERROR, L"invalid custom property length" );
		return false;
	}

	if( !openFileExclusively( aFilePath, aError ) )
	{
		return false;
	}

	HZIP hZIP = OpenZip( aFilePath.c_str(), NULL );

	if( hZIP == NULL )
	{
		if( setDlpStorageCustomProperties( aFilePath, aCustomProperties ) )
		{
			return true;
		}
		else
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"Failed to open zip file: %s", aFilePath.c_str() );
			return false;
		}
	}
	
	CloseZip( hZIP );

	std::wstring tempFileName;
	if( !copyFileToTemp( aFilePath, aError, tempFileName ) )
	{
		return false;
	}

	hZIP = OpenZip( tempFileName.c_str(), NULL );

	if( hZIP == NULL )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to open zip file: %s", aFilePath.c_str() );
		DeleteFile( tempFileName.c_str() );
		return false;
	}

	StringPairList customXmls;
	if( !createCustomXmls( hZIP, customXmls, aCustomProperties ) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to create custom XMLs for file: %s", aFilePath.c_str() );
		CloseZip( hZIP );
		DeleteFile( tempFileName.c_str() );
		return false;
	}

	CloseZip( hZIP );

	for( auto itr = customXmls.begin(); itr != customXmls.end(); ++itr )
	{
		if( AddFileToZip( tempFileName.c_str() , itr->first.c_str(), NULL, (char*)itr->second.c_str(), (int)itr->second.length() ) != ZR_OK )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"Failed to add file to zip file for: %s", aFilePath.c_str() );
			DeleteFile( tempFileName.c_str() );
			return false;
		}
	}

	if( !MoveFileEx( tempFileName.c_str(), aFilePath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED ) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to replace file: %s. Error %d", aFilePath.c_str(), GetLastError() );
		aError = MT_ERROR_FILE_LOCKED;
		return false;
	}
	
	aError = MT_ERROR_SUCCESS;

	return true;
}

bool DocumentPropertiesService::getDlpStorageCustomProperties( 	
	const std::wstring& aFilePath,
	HANDLE aFileHandle,
	DLPCustomProperties& aCustomProperties )
{
	IPropertySetStorage *pPropSetStg;
	IPropertyStorage *pPropStg;
	IStorage* iStorage = NULL;
	CILockBytesOnFileHandle lockBytes;
	HRESULT hr;

	if( aFileHandle == INVALID_HANDLE_VALUE )
	{
		hr = StgOpenStorageEx( aFilePath.c_str(), STGM_READ|STGM_SHARE_DENY_WRITE, STGFMT_DOCFILE, 0, NULL, NULL, IID_IPropertySetStorage, (void**)&pPropSetStg );

		if( FAILED(hr) )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"StgOpenStorageEx failed: %d", hr );
			return false;
		}
	}
	else
	{
		lockBytes.HrInit( aFileHandle );

		hr = StgIsStorageILockBytes( &lockBytes );

		if( FAILED(hr) )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"StgIsStorageILockBytes failed: %d", hr );
			return false;
		}

		hr = StgOpenStorageOnILockBytes( &lockBytes, NULL, STGM_READ|STGM_TRANSACTED|STGM_SHARE_DENY_NONE, NULL, NULL, &iStorage );

		if( FAILED(hr) )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"StgOpenStorageOnILockBytes failed: %d", hr );
			return false;
		}

		hr = iStorage->QueryInterface( IID_IPropertySetStorage, (void**)&pPropSetStg );

		if( FAILED(hr) )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"QueryInterface IID_IPropertySetStorage failed: %d", hr );
			iStorage->Release();
			return false;
		}
	}

	hr = pPropSetStg->Open(FMTID_UserDefinedProperties, STGM_READ|STGM_SHARE_EXCLUSIVE, &pPropStg);

	if( hr != S_OK )
	{
		pPropSetStg->Release();

		if( iStorage )
		{
			iStorage->Release();
		}
		
		if( hr == STG_E_FILENOTFOUND )
		{
			return true;
		}

		OLOG( OLoggingManager::OSEV_ERROR, L"Failed opening user properties: %d", hr );
		return false;
	}

	PROPSPEC propspec[6]; 
	PROPVARIANT propvarRead[6]; 

	propspec[0].ulKind = PRSPEC_LPWSTR;
	propspec[0].lpwstr = CUSTOM_PROPERTY_CLASSIFICATIONS;
	propspec[1].ulKind = PRSPEC_LPWSTR;
	propspec[1].lpwstr = CUSTOM_PROPERTY_CLASSIFIER;
	propspec[2].ulKind = PRSPEC_LPWSTR;
	propspec[2].lpwstr = CUSTOM_PROPERTY_DATE;
	propspec[3].ulKind = PRSPEC_LPWSTR;
	propspec[3].lpwstr = CUSTOM_PROPERTY_AUTO_CLASSIFICATIONS;
	propspec[4].ulKind = PRSPEC_LPWSTR;
	propspec[4].lpwstr = CUSTOM_PROPERTY_MANUAL_VERSION;
	propspec[5].ulKind = PRSPEC_LPWSTR;
	propspec[5].lpwstr = CUSTOM_PROPERTY_AUTO_VERSION;

	hr = pPropStg->ReadMultiple( 6, propspec, propvarRead );

	if( hr != S_OK )
	{
		pPropStg->Release();
		pPropSetStg->Release();

		if( iStorage )
		{
			iStorage->Release();
		}

		if( hr == S_FALSE )
		{
			return true;
		}

		OLOG( OLoggingManager::OSEV_ERROR, L"Failed reading properties: %d", hr );
		return false;
	}
	
	if( propvarRead[0].pwszVal )
	{
		aCustomProperties.mClassifications = propvarRead[0].pwszVal;
	}
	if( propvarRead[1].pwszVal )
	{
		aCustomProperties.mClassifier = propvarRead[1].pwszVal;
	}
	if( propvarRead[2].pwszVal )
	{
		aCustomProperties.mDate = propvarRead[2].pwszVal;
	}
	if( propvarRead[3].pwszVal )
	{
		aCustomProperties.mAutoClassifications = propvarRead[3].pwszVal;
	}
	if( propvarRead[4].pwszVal )
	{
		aCustomProperties.mManualVersion = propvarRead[4].pwszVal;
	}
	if( propvarRead[5].pwszVal )
	{
		aCustomProperties.mAutoVersion = propvarRead[5].pwszVal;
	}

	pPropStg->Release();
	pPropSetStg->Release();

	if( iStorage )
	{
		iStorage->Release();
	}

	return true;
}

bool DocumentPropertiesService::getDlpCustomProperties( 
	const std::wstring& aFilePath,
	HANDLE aFileHandle,
	DLPCustomProperties& aCustomProperties,
	ManualClassificationError& aError )
{
	aError = MT_ERROR_INTERNAL;

	HZIP hZIP;

	HANDLE hFile = INVALID_HANDLE_VALUE;

	if( aFileHandle != INVALID_HANDLE_VALUE )
	{
		hZIP = OpenZipHandle( aFileHandle, NULL );
	}
	else
	{
		hFile = CreateFile( aFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL );

		if( hFile == INVALID_HANDLE_VALUE )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"Failed to open file: %s. Error %d", aFilePath.c_str(), GetLastError() );
			aError = GetLastError() == ERROR_SHARING_VIOLATION ? MT_ERROR_FILE_LOCKED : MT_ERROR_ACCESS_FAILED;
			return false;
		}

		hZIP = OpenZipHandle( hFile, NULL );
	}

	if( NULL == hZIP )
	{
		if( hFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle( hFile );
		}

		if( getDlpStorageCustomProperties( aFilePath, aFileHandle, aCustomProperties ) )
		{
			return true;
		}

		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to open zip file: %s", aFilePath.c_str() );
		return false;
	}

	ZIPENTRY ze; 
	int index;
	ZRESULT zr = FindZipItem( hZIP, L"docProps/custom.xml", true, &index, &ze );
	
	if( zr == ZR_NOTFOUND )
	{
		CloseZip( hZIP );
		if( hFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle( hFile );
		}
		return true;
	}

	if( zr != ZR_OK )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Error when looking for custom.xml in file: %s", aFilePath.c_str() );
		CloseZip( hZIP );
		if( hFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle( hFile );
		}
		return false;
	}

	_bstr_t customXml;

	if ( !getXMLFromZipFile( hZIP, ze, index, customXml ) )
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to get custom.xml in file: %s", aFilePath.c_str() );
		CloseZip( hZIP );
		if( hFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle( hFile );
		}
		return false;
	}

	CloseZip( hZIP );

	if( hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( hFile );
	}

	MSXML2::IXMLDOMDocumentPtr xmlDOMObject;

	if( xmlDOMObject.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK )
	{
		return false;
	}

	if ( false == xmlDOMObject->loadXML( customXml ) ) 
	{
		return false;
	}

	MSXML2::IXMLDOMNodePtr propertiesNode = xmlDOMObject->selectSingleNode( L"Properties" );

	if( nullptr == propertiesNode )
	{
		return false;
	}

	MSXML2::IXMLDOMNodePtr propertyNode = propertiesNode->selectSingleNode( L"property[@name=\"" CUSTOM_PROPERTY_CLASSIFICATIONS L"\"]" );
	if( propertyNode )
	{
		aCustomProperties.mClassifications = getNodeValue( propertyNode, L"vt:lpwstr" );
	}

	propertyNode = propertiesNode->selectSingleNode( L"property[@name=\"" CUSTOM_PROPERTY_CLASSIFIER L"\"]" );
	if( propertyNode )
	{
		aCustomProperties.mClassifier = getNodeValue( propertyNode, L"vt:lpwstr" );
	}

	propertyNode = propertiesNode->selectSingleNode( L"property[@name=\"" CUSTOM_PROPERTY_DATE L"\"]" );
	if( propertyNode )
	{
		aCustomProperties.mDate = getNodeValue( propertyNode, L"vt:lpwstr" );
	}

	propertyNode = propertiesNode->selectSingleNode( L"property[@name=\"" CUSTOM_PROPERTY_AUTO_CLASSIFICATIONS L"\"]" );
	if( propertyNode )
	{
		aCustomProperties.mAutoClassifications = getNodeValue( propertyNode, L"vt:lpwstr" );
	}

	propertyNode = propertiesNode->selectSingleNode( L"property[@name=\"" CUSTOM_PROPERTY_MANUAL_VERSION L"\"]" );
	if( propertyNode )
	{
		aCustomProperties.mManualVersion = getNodeValue( propertyNode, L"vt:lpwstr" );
	}

	propertyNode = propertiesNode->selectSingleNode( L"property[@name=\"" CUSTOM_PROPERTY_AUTO_VERSION L"\"]" );
	if( propertyNode )
	{
		aCustomProperties.mAutoVersion = getNodeValue( propertyNode, L"vt:lpwstr" );
	}

	aError = MT_ERROR_SUCCESS;

	return true;
}

void DocumentPropertiesService::setXmpProperty( 
	const std::wstring& aName, 
	const std::wstring& aValue, 
	SXMPMeta& aXMPMeta )
{
	if( aValue == IGNORE_PROPERTY_VALUE )
	{
		return;
	}

	char* nameUTF8;
	if( StringUtilities::wideCharToUtf8( aName, nameUTF8 ) == -1 )
	{
		return;
	}

	char* valueUTF8;
	if( StringUtilities::wideCharToUtf8( aValue, valueUTF8 ) == -1 )
	{
		delete [] nameUTF8;
		return;
	}

	if( aValue.empty() )
	{
		aXMPMeta.DeleteProperty( kXMP_NS_XMP, nameUTF8 );
	}
	else
	{
		aXMPMeta.SetProperty( kXMP_NS_XMP, nameUTF8, valueUTF8, NULL );
	}

	delete [] nameUTF8;
	delete [] valueUTF8;
}

bool DocumentPropertiesService::setDlpXmpProperties( 
	const std::wstring& aFilePath,
	const DLPCustomProperties& aCustomProperties,
	ManualClassificationError& aError )
{
	aError = MT_ERROR_INTERNAL;

	std::wstring tempFileName;
	FileUnlinker tempFileUnlinker(L"");

	if( isPDF( aFilePath, INVALID_HANDLE_VALUE ) )
	{
		if( !openFileExclusively( aFilePath, aError ) )
		{
			return false;
		}

		//PDF_Handler in XMP SDK fails when file name contains non-english characters, so we copy the file and rename it.
		if( !copyFileToTemp( aFilePath, aError, tempFileName ) )
		{
			return false;
		}

		tempFileUnlinker.mFileName = tempFileName;
	}

	char* fileNameUTF8;
	if( StringUtilities::wideCharToUtf8( tempFileName.empty() ? aFilePath : tempFileName, fileNameUTF8 ) == -1 )
	{
		return false;
	}
	
	SXMPFiles xmpFile;

	try
	{
		xmpFile.OpenFile( fileNameUTF8, kXMP_UnknownFile, kXMPFiles_OpenForUpdate | kXMPFiles_OpenUseSmartHandler );

		delete [] fileNameUTF8;
		fileNameUTF8 = NULL;

		SXMPMeta meta;
		xmpFile.GetXMP( &meta );

		setXmpProperty( CUSTOM_PROPERTY_CLASSIFICATIONS, aCustomProperties.mClassifications, meta );
		setXmpProperty( CUSTOM_PROPERTY_CLASSIFIER, aCustomProperties.mClassifier, meta );
		setXmpProperty( CUSTOM_PROPERTY_DATE, aCustomProperties.mDate, meta );
		setXmpProperty( CUSTOM_PROPERTY_AUTO_CLASSIFICATIONS, aCustomProperties.mAutoClassifications, meta );
		setXmpProperty( CUSTOM_PROPERTY_MANUAL_VERSION, aCustomProperties.mManualVersion, meta );
		setXmpProperty( CUSTOM_PROPERTY_AUTO_VERSION, aCustomProperties.mAutoVersion, meta );

		if( !xmpFile.CanPutXMP(meta) )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"Cannot add XMP metadata to file: %s", aFilePath.c_str() );
			xmpFile.CloseFile();
			return false;
		}

		xmpFile.PutXMP(meta);
		xmpFile.CloseFile();
	}
	catch(XMP_Error& e)
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to modify XMP for file: %s. Error %S", aFilePath.c_str(), e.GetErrMsg() );

		if( e.GetID() != kXMPErr_RejectedFileExtension && e.GetID() != kXMPErr_NoFileHandler )
		{
			aError = e.GetID() == kXMPErr_ExternalFailure ? MT_ERROR_FILE_LOCKED : MT_ERROR_ACCESS_FAILED;
		}

		if( fileNameUTF8 )
		{
			delete [] fileNameUTF8;
		}

		xmpFile.CloseFile();

		return false;
	}

	if( !tempFileName.empty() )
	{
		if( !MoveFileEx( tempFileName.c_str(), aFilePath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED ) )
		{
			OLOG( OLoggingManager::OSEV_ERROR, L"Failed to replace file: %s. Error %d", aFilePath.c_str(), GetLastError() );
			aError = MT_ERROR_FILE_LOCKED;
			return false;
		}

		tempFileUnlinker.mFileName = L"";
	}

	aError = MT_ERROR_SUCCESS;

	return true;
}

bool DocumentPropertiesService::isPDF( const std::wstring& aFilePath, HANDLE aFileHandle )
{
	if( aFileHandle != INVALID_HANDLE_VALUE )
	{
		//check by signature
		const BYTE pdfSignature[4] = {0x25,0x50,0x44,0x46};

		BYTE fileSignature[4] = {0};
		DWORD dwRead;
		if( !ReadFile( aFileHandle, fileSignature, 4, &dwRead, NULL ) )
		{
			return false;
		}

		return memcmp( pdfSignature, fileSignature, 4 ) == 0;
	}

	//check by extension
	std::wstring filePath = aFilePath;
	StringUtils::toLower( filePath );

	return filePath.size() > 4 && filePath.find( L".pdf", filePath.size() - 4 ) != std::wstring::npos;
}

std::wstring DocumentPropertiesService::findCustomProperty( const std::string& aContent, LPCSTR aPrefix, LPCSTR aSuffix )
{
	size_t startPos = aContent.find( aPrefix );

	if( startPos != std::string::npos )
	{
		startPos += strlen( aPrefix );

		size_t endPos = aContent.find( aSuffix, startPos );

		if( endPos != std::string::npos )
		{
			std::wstring customProperty;
			StringUtils::string2wstring( aContent.substr( startPos, endPos-startPos ), customProperty );
			return customProperty;
		}
	}

	return L"";
}

void DocumentPropertiesService::getXmpProperty( 
	const std::wstring& aName, 
	std::wstring& aValue, 
	const SXMPMeta& aXMPMeta )
{
	char* nameUTF8;
	if( StringUtilities::wideCharToUtf8( aName, nameUTF8 ) == -1 )
	{
		return;
	}

	std::string valueUTF8;
	if( aXMPMeta.GetProperty( kXMP_NS_XMP, nameUTF8, &valueUTF8, NULL ) )
	{
		WCHAR* valueUnicode;
		if( !StringUtilities::utf8ToWideChar( (char*)valueUTF8.c_str(), -1, valueUnicode ) )
		{
			delete [] nameUTF8;
			return;
		}

		aValue = valueUnicode;
		delete [] valueUnicode;
	}

	delete [] nameUTF8;
}

bool DocumentPropertiesService::getDlpXmpProperties( 
	const std::wstring& aFilePath,
	HANDLE& aFileHandle,
	DLPCustomProperties& aCustomProperties,
	ManualClassificationError& aError )
{
	aError = MT_ERROR_INTERNAL;

	XMP_OptionBits openFlags = kXMPFiles_OpenForRead;

	//PDF_Handler in XMP SDK fails when reading by handle or when file name contains non-english characters, so we use packet scanning.
	openFlags |= isPDF( aFilePath, aFileHandle ) ? kXMPFiles_OpenUsePacketScanning : kXMPFiles_OpenUseSmartHandler;

	char* fileNameUTF8;
	if( StringUtilities::wideCharToUtf8( aFilePath, fileNameUTF8 ) == -1 )
	{
		return false;
	}

	XMPFiles_IO* xmpIO = NULL;

	SXMPFiles xmpFile;

	try
	{
		if( aFileHandle == INVALID_HANDLE_VALUE )
		{
			xmpFile.OpenFile( fileNameUTF8, kXMP_UnknownFile, openFlags );
		}
		else
		{
			xmpIO = new XMPFiles_IO( aFileHandle, fileNameUTF8, true );

			xmpFile.OpenFile( xmpIO, kXMP_UnknownFile, openFlags );
		}

		SXMPMeta meta;
		xmpFile.GetXMP( &meta );

		getXmpProperty( CUSTOM_PROPERTY_CLASSIFICATIONS, aCustomProperties.mClassifications, meta );
		getXmpProperty( CUSTOM_PROPERTY_CLASSIFIER, aCustomProperties.mClassifier, meta );
		getXmpProperty( CUSTOM_PROPERTY_DATE, aCustomProperties.mDate, meta );
		getXmpProperty( CUSTOM_PROPERTY_AUTO_CLASSIFICATIONS, aCustomProperties.mAutoClassifications, meta );
		getXmpProperty( CUSTOM_PROPERTY_MANUAL_VERSION, aCustomProperties.mManualVersion, meta );
		getXmpProperty( CUSTOM_PROPERTY_AUTO_VERSION, aCustomProperties.mAutoVersion, meta );

		xmpFile.CloseFile();
	}
	catch(XMP_Error& e)
	{
		OLOG( OLoggingManager::OSEV_ERROR, L"Failed to get XMP for file: %s. Error %S", aFilePath.c_str(), e.GetErrMsg() );

		if( e.GetID() != kXMPErr_RejectedFileExtension && e.GetID() != kXMPErr_NoFileHandler )
		{
			aError = e.GetID() == kXMPErr_ExternalFailure ? MT_ERROR_FILE_LOCKED : MT_ERROR_ACCESS_FAILED;
		}

		delete [] fileNameUTF8;
		delete xmpIO;

		xmpFile.CloseFile();

		//xmpIO destructor closes the handle
		aFileHandle = INVALID_HANDLE_VALUE;

		return false;
	}

	delete [] fileNameUTF8;
	delete xmpIO;
	//xmpIO destructor closes the handle
	aFileHandle = INVALID_HANDLE_VALUE;

	aError = MT_ERROR_SUCCESS;

	return true;
}
