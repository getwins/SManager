/********************************************************************
	created:	2006/08/23
	created:	23:8:2006   9:41
	filename: 	D:\proj\ksbass\bcclink\bccclt\bccclt.h
	file path:	D:\proj\ksbass\bcclink\bccclt
	file base:	bccclt
	file ext:	h
	author:		CHENYH
	
	purpose:	���ͷ�ļ�Ŀ����Ϊ���ܹ����ɿ�����C��C++Ҳ�ܵ��õĽӿ�

   2009-5-12 14:36:52 ����BCGetPCharFieldByName��������BCGetStringFieldByName��unsigned char�ֶ�������ͬ
   
   2010-4-26 21:56:57 : ��ʹ��KLG��־�����ϣ�ӦXZCҪ�󣬼���BCKLGMsg�����ӿ�
     
   2012/3/15 16:16:50 : ����XPACK�������У���EncodeRow�и�Ϊʵ�ʱ����жϷ�����������
       
   2012/08/29 : ����BCCallRequestEx����  -- bCheckUserData����
         
   2013/10/29 16:19:17 Ϊ�˾������յ�������ʱ�������Ӧ�������������������/Ӧ������
      �ڸ���ԭ���ӵ�ʱ�򣬶����ӽ��д��������ݵ���մ��� T20131029
           
   2014/7/28 21:17:39 ԭ�����������⣺
      1. KSLog.cpp����һ������WriteTxtLogû�в���MUTEX������ֱ�����µ�KSLog.cpp
      2. ��CDrtpRouter::CLISendReceive�У�����һ��break�����½�����ѭ��״̬����������
      ����BCCallRequestEx��ʱ�򣬼������
             
   2015/3/18 18:03:54 �������º�����V2.37
      BCCallRequest1(...); 
      BCCallRequestEx1(...);
      BCSetRecvClearWait(...);   
*********************************************************************/
#ifndef NBCCCLT_H
#define NBCCCLT_H

#ifdef _USRDLL
#ifdef BCCCLT_EXPORTS
#define BCCCLT_API __declspec(dllexport)
#else
#define BCCCLT_API __declspec(dllimport)
#endif
#else
#define BCCCLT_API
#endif

#ifndef _XPCLT_H_INCLUDED_
typedef void * BCHANDLE;
#endif

#define VERSION_DATE "BCCCLT Version 2.37 by CHENYH @ 2015/3/18 17:52:09"

#ifdef __cplusplus
extern "C" {
#else
   typedef unsigned char bool;
/* ****** Updated by CHENYH at 2006-7-12 11:04:58 ****** 
   #define true 1
   #define false 0
*******************************************************/
#endif

   // Function name: BCSetRecvClearWait
   // Author: CHENYH @ 2015/3/18 17:52:09
   // Description: ����������ջ�������ʱ�� -- Ϊ�˰�ȫ���ٶȵ�Ҫ��
   //   ˵��: �ڸ���ͨѶ������·������£�Ϊ�˱��������ϲ����ϴ�û�н��յ����ݣ�BCCCLT�ڲ����һ��ʱ��ȥ���ղ������ݣ��ȴ�ʱ�伴ΪmsRBufClearWait, Ĭ��Ϊ10ms ����Windows��ʵ�ʹ���Ϊ15ms��
   //         ����ʹ�÷��ܹ�ȷ��������ֲ������ݵ�����£������ñ���������Ϊ��߷���������ٶ�
   // Return type: void 
   // Argument   : int msRBufClearWait [IN]: ����ƵĽ��ջ�������ʱ��
   BCCCLT_API  void BCSetRecvClearWait(int msRBufClearWait);

   // ����  : ���Ի���ϵͳ��ֱ�����Ӷ��ٸ�ҵ��ͨѶƽ̨�ڵ����
   //		Notice: ϵͳֻ�ܵ���һ��
   // ����  : bool 
   // ����  : int maxnodes=1 [IN]: ���������ж��ٸ����ӽڵ�
   BCCCLT_API  bool BCCCLTInit(int maxnodes);
   
   // ����  : ����һ��Ҫֱ�����ӵ�ҵ��ͨѶƽ̨�ڵ�
   //       ��Ϊϵͳ���Ի����֣������ڵ���BCCCLTInit����ͳһһ���߳���һ���Ե�������ϣ�����������ʹ��
   // ����  : int  ���ӵ�ҵ��ͨѶƽ̨�ڵ��� >=0:�ɹ������򣬿���û����ȷ����BCCCLTInit()
   // ����  : char * ip [IN]: ��ҵ��ͨѶƽ̨��IP��ַ
   // ����  : int port [IN]: ��ҵ��ͨѶƽ̨�ṩ�Ķ˿ں�port
   BCCCLT_API  int BCAddDrtpNode(char * ip,int port);


   // ����  : ����һ������XPack���ݽ�����������
   // ����  : BCHANDLE : ���ظ�XPack�������ľ����==0(NULL): ʧ��
   // ����  : const char * XpackDescribleFile [IN]: XPack�ṹ�����ļ��������ܳ���1024�ֽڳ�����: "/xpacks/cpack.dat"
	BCCCLT_API  BCHANDLE BCNewHandle(const char * XpackDescribleFile);
   
   // ����  : ���Ѿ���BCNewHandle�õ����������ͷ�(ж��)
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   BCCCLT_API  bool BCDeleteHandle(BCHANDLE Handle);

   // ����  : �������������ݣ���Ҫ����׼���װ�����ʱ����д�������������ֶ�֮ǰ����
   // ����  : bool : false - ����Ϊ�����Handle(==NULL��ʱ��)
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
	BCCCLT_API  bool BCResetHandle(BCHANDLE Handle);

   // ����  : ��SourceHandle�е�״̬����ʽ�����ݵ���Ϣ���Ƶ�DestHandle��
   // ����  : bool : false - ����Ϊ�����Handle(==NULL��ʱ��)
   // ����  : BCHANDLE SourceHandle [IN]: �����Ƶ�ԭXPack���������
   // ����  : BCHANDLE DestHandle   [OUT]: �����Ƶ�Ŀ��XPack������������ڲ����ݱ����ǣ�������������һ�����
	BCCCLT_API  bool BCCopyHandle(BCHANDLE SourceHandle,BCHANDLE DestHandle);
//--------------------------------------------------------------------------------------------------
   //////////////////////////////////////////////////////////////////////////
   // ��ͨ��BCNewHandle�õ�����󣬾Ϳ������������ֶ������ˣ�

   // ����  : ����ָ���������е�Row����¼�еĶ�Ӧ�ֶε�ֵ
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ������ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
   // ����  : int Value [IN]: ����ֵ
   BCCCLT_API  bool BCSetIntFieldByName(BCHANDLE Handle,int Row,char * FieldName,int Value);
   
   // ����  : ����ָ���������е�Row����¼�еĶ�Ӧ�ֶε�ֵ
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ������ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
   // ����  : double Value [IN]: ����ֵ
   BCCCLT_API  bool BCSetDoubleFieldByName(BCHANDLE Handle,int Row,char * FieldName,double Value);
   
   // ����  : ����ָ���������е�Row����¼�еĶ�Ӧ�ֶε�ֵ
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ������ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
   // ����  : char * Value [IN]: ����ֵ�������Գ���255����
   BCCCLT_API  bool BCSetStringFieldByName(BCHANDLE Handle,int Row,char * FieldName,char * Value);
      
   
   // ����  : ����ָ����¼Ϊָ����RawData��¼
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ָ�����õļ�¼�� [0..RecordCount-1]
   // ����  : char * RawData [IN]: Ҫ�������ݿ�
   // ����  : int RawDataLength [IN]: Ҫ�������ݿ鳤��
   BCCCLT_API  bool BCSetRawRecord(BCHANDLE Handle, int Row, char * RawData, int RawDataLength);


   // ����  : �������������еĹ��ܺ� ����EncodeXpackForRequest֮ǰһ��Ҫ���ã�
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int RequestType [IN]: ��Ӧ��ҵ���ܺ�
   BCCCLT_API  bool BCSetRequestType(BCHANDLE Handle,int RequestType);

   
   // ����  : ����BCC��ķ��������ͻ��˿�������Ӧ���ÿ�η��ص�����¼��
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int MaxRetCount [IN]: ָ��������¼����0����Ӧ�÷������� [0..100]; 
   BCCCLT_API  bool BCSetMaxRetCount(BCHANDLE Handle,int MaxRetCount);
   

   // 2014/1/22 16:44:28 ��������ԭʼ���󷽵�Mac��ַ��IP��ַ��raw���ݣ�
   // Function name: BCSetAddress
   // Author: CHENYH @ 2014/7/9 10:55:09
   // Description: �������������ָ���Ļ�����ַ���Թ��������˲�����
   // Return type: bool 
   // Argument   : BCHANDLE Handle
   // Argument   : void *pAddress [IN]: ��������ֽڣ���Mac��ַ��IP��ַ
   BCCCLT_API  bool BCSetAddress(BCHANDLE Handle, void *pAddress);

   //////////////////////////////////////////////////////////////////////////
   // ���������������ݺ������ܺţ��������Ϳ��������к�����������������ˣ�

   // ����  : ��ǰ�����ú�XPack�������ݴ�����͸�ָ����ͨѶƽ̨��Ӧ�÷�����
   // ����  : bool : true �ɹ����ͣ����ҽ��ս�����Ӧ�����ݣ�false ����ʧ�ܣ�������󣬲μ�������
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int drtpno [IN]: ���Ǹ�ֱ����ҵ��ͨѶƽ̨�ڵ㷢�ͺͽ������ݣ��μ�BCAddDrtpNode()�ķ���ֵ
   // ����  : int branchno [IN]: Ŀ��Ӧ�÷��������ӵ�ҵ��ͨѶƽ̨�ڵ��ţ��μ�ҵ��ͨѶƽ̨�����ò�����
   // ����  : int function [IN]: Ŀ��Ӧ�÷�����ע����ҵ��ͨѶƽ̨�Ĺ��ܺ� (�μ�BCC�����ļ��е�BASEFUNCNO)
   // ����  : int timeout [IN]: ���ͽ������ݵĳ�ʱʱ�䣬�Ժ����
   // ����  : int* errcode [OUT]: ���صĴ�����
   // ����  : char * errormessage [OUT]: ���صĴ�����Ϣ
   BCCCLT_API  bool BCCallRequest(BCHANDLE Handle, int drtpno, int branchno,int function, int timeout, int *errcode,char * errormessage);
   

   // Function name: bool BCCallRequest1
   // Author: CHENYH @ 2015/3/18 11:16:45
   // ����  : ��ǰ�����ú�XPack�������ݴ�����͸�ָ����ͨѶƽ̨��Ӧ�÷�����
   // ����  : bool : true �ɹ����ͣ����ҽ��ս�����Ӧ�����ݣ�false ����ʧ�ܣ�������󣬲μ�������
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int drtpno [IN]: ���Ǹ�ֱ����ҵ��ͨѶƽ̨�ڵ㷢�ͺͽ������ݣ��μ�BCAddDrtpNode()�ķ���ֵ
   // ����  : int branchno [IN]: Ŀ��Ӧ�÷��������ӵ�ҵ��ͨѶƽ̨�ڵ��ţ��μ�ҵ��ͨѶƽ̨�����ò�����
   // ����  : int function [IN]: Ŀ��Ӧ�÷�����ע����ҵ��ͨѶƽ̨�Ĺ��ܺ� (�μ�BCC�����ļ��е�BASEFUNCNO)
   // ����  : int timeout [IN]: ���ͽ������ݵĳ�ʱʱ�䣬�Ժ����
   // ����  : int* errcode [OUT]: ���صĴ�����
   // ����  : char * errormessage [OUT]: ���صĴ�����Ϣ
   // Argument   : bool bToLog [IN]: �Ƿ���Ҫ��������Ӧ�����ݼ�¼��klg��־�ļ���? false: ����¼
   BCCCLT_API  bool BCCallRequest1(BCHANDLE Handle, int drtpno, int branchno,int function, int timeout, int *errcode,char * errormessage, bool bToLog);

   // Function name: bool BCCallRequestEx
   // Author: CHENYH @ 2012/8/29 10:50:57
   // ����  : ��ǰ�����ú�XPack�������ݴ�����͸�ָ����ͨѶƽ̨��Ӧ�÷�����
   // ����  : bool : true �ɹ����ͣ����ҽ��ս�����Ӧ�����ݣ�false ����ʧ�ܣ�������󣬲μ�������
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int drtpno [IN]: ���Ǹ�ֱ����ҵ��ͨѶƽ̨�ڵ㷢�ͺͽ������ݣ��μ�BCAddDrtpNode()�ķ���ֵ
   // ����  : int branchno [IN]: Ŀ��Ӧ�÷��������ӵ�ҵ��ͨѶƽ̨�ڵ��ţ��μ�ҵ��ͨѶƽ̨�����ò�����
   // ����  : int function [IN]: Ŀ��Ӧ�÷�����ע����ҵ��ͨѶƽ̨�Ĺ��ܺ� (�μ�BCC�����ļ��е�BASEFUNCNO)
   // ����  : int timeout [IN]: ���ͽ������ݵĳ�ʱʱ�䣬�Ժ����
   // ����  : int* errcode [OUT]: ���صĴ�����
   // ����  : char * errormessage [OUT]: ���صĴ�����Ϣ
   // Argument   : bool bCheckUserData [IN]: ָ���Ƿ����userdata��飬true-���飬����false(0)�����
   BCCCLT_API  bool BCCallRequestEx(BCHANDLE Handle, int drtpno, int branchno,int function, int timeout, int *errcode,char * errormessage,bool bCheckUserData);


   // Function name: BCCallRequestEx1
   // Author: CHENYH @ 2015/3/18 11:21:00
   // ����  : ��ǰ�����ú�XPack�������ݴ�����͸�ָ����ͨѶƽ̨��Ӧ�÷�����
   // ����  : bool : true �ɹ����ͣ����ҽ��ս�����Ӧ�����ݣ�false ����ʧ�ܣ�������󣬲μ�������
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int drtpno [IN]: ���Ǹ�ֱ����ҵ��ͨѶƽ̨�ڵ㷢�ͺͽ������ݣ��μ�BCAddDrtpNode()�ķ���ֵ
   // ����  : int branchno [IN]: Ŀ��Ӧ�÷��������ӵ�ҵ��ͨѶƽ̨�ڵ��ţ��μ�ҵ��ͨѶƽ̨�����ò�����
   // ����  : int function [IN]: Ŀ��Ӧ�÷�����ע����ҵ��ͨѶƽ̨�Ĺ��ܺ� (�μ�BCC�����ļ��е�BASEFUNCNO)
   // ����  : int timeout [IN]: ���ͽ������ݵĳ�ʱʱ�䣬�Ժ����
   // ����  : int* errcode [OUT]: ���صĴ�����
   // ����  : char * errormessage [OUT]: ���صĴ�����Ϣ
   // Argument   : bool bToLog [IN]: �Ƿ���Ҫ��������Ӧ�����ݼ�¼��klg��־�ļ���? false: ����¼
   bool BCCallRequestEx1(BCHANDLE Handle, int drtpno, int branchno,int function, int timeout, int *errcode,char * errormessage,bool bCheckUserData, bool bToLog);

   // ������: BCPush0
   // ���  : ������ 2010-8-16 16:39:34
   // ����  : ��ǰ�����úõ�XPack�������ݴ����PM0������ȷ��ģʽ�����͸�ָ����ͨѶƽ̨�ͷ����
   // ����  : bool : true ���ͳɹ���false ����ʧ�ܣ��������μ�errormessage
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int drtpno [IN]: ���Ǹ�ֱ����ҵ��ͨѶƽ̨�ڵ㷢�ͺͽ������ݣ��μ�BCAddDrtpNode()�ķ���ֵ
   // ����  : int branchno [IN]: Ŀ��������͵ķ�������ӵ�ҵ��ͨѶƽ̨�ڵ��ţ��μ�ҵ��ͨѶƽ̨�����ò�����
   // ����  : int function [IN]: Ŀ��������͵ķ����ע����ҵ��ͨѶƽ̨�Ĺ��ܺ� (�μ�BCC�����ļ��е�BASEFUNCNO)
   // ����  : int *errcode [OUT]: ���صĴ����룬>=0: �ɹ�������(<0)ʧ��
   // ����  : char * errormessage [OUT]: ���صĴ�����Ϣ
   BCCCLT_API  bool BCPush0(BCHANDLE Handle, int drtpno, int branchno,int function, int *errcode,char * errormessage);

   
   //////////////////////////////////////////////////////////////////////////
   //�ڵ�����BCCallRequest�󣬾�Ҫ��鷵�ص��������ݣ�
   
   // ����  : �ڵ���BCCallRequest���ȡ�䷵����
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int* RetCode [OUT]: �����յ����ݰ��еķ�����
   BCCCLT_API  bool BCGetRetCode(BCHANDLE Handle,int * RetCode);

   // ����  : �ڵ���BCCallRequest/BCCallNext���ȡ�����еļ�¼��RecordCount
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int * RecordCount [OUT]: ���ؽ����ļ�¼��
   BCCCLT_API  bool BCGetRecordCount(BCHANDLE Handle,int * RecordCount);   
   
   // ����  : �����յý��������ݣ����еõ����ڿ��Ի�ȡ����Ӧ�����ݰ���Ӧ�÷�����ר��ͨѶ���ܺ�
   //          ��ϸ������ο�KSBCCʹ��˵����
   // ����  : int : >0 - �ɻ�ȡ�������ݵ�ר��ͨѶ���ܺţ�����ʧ��
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   BCCCLT_API  int BCGetPrivateFunctionForNext(BCHANDLE Handle);
   
   // ����  : �����յý��������ݣ����еõ����ڿ��Ի�ȡ����Ӧ�����ݰ���Ŀ��ͨѶƽ̨�ڵ���
   //          ��ϸ������ο�KSMBCCʹ��˵����
   // ����  : int >0: ��ȡ����Ӧ�����ݰ���Ŀ��ͨѶƽ̨�ڵ���; ��������ԭ���Ľڵ���
   //                ���ù�������Transferʵ�ֵ�ʱ����Ҫ������ܱ�����˵Ľڵ��
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   BCCCLT_API  int BCGetBranchNoForNext(BCHANDLE Handle);
   
   // ���  : ������ 2005-11-14 22:01:15
   // ����  : �������ͨ���������ж��Ƿ��к���Ӧ���
   // ����  : bool : true-��ʾ���к���Ӧ�����false���޺�����
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   BCCCLT_API  bool BCHaveNextPack(BCHANDLE Handle);

   // ����  : ȡ��һ������Ӧ���
   //          �ڳɹ�����BCCallRequest�������鵽BCHaveNextPack()�����ñ����ܻ�ȡ��һ������Ӧ���
   // ����  : bool ��true �ɹ����ͣ����ҽ��ս�����Ӧ�����ݣ�false ����ʧ�ܣ�������󣬲μ�������
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int timeout [IN]: ���ͽ������ݵĳ�ʱʱ�䣬�Ժ����
   // ����  : int* errcode [OUT]: ���صĴ�����
   // ����  : char * errormessage [OUT]: ���صĴ�����Ϣ
   BCCCLT_API  bool BCCallNext(BCHANDLE Handle, int timeout, int* errcode,char * errormessage);


   // Function name: BCCallNextEx
   // Author: CHENYH @ 2012/8/29 10:53:34
   // ����  : ȡ��һ������Ӧ���
   //          �ڳɹ�����BCCallRequest�������鵽BCHaveNextPack()�����ñ����ܻ�ȡ��һ������Ӧ���
   // ����  : bool ��true �ɹ����ͣ����ҽ��ս�����Ӧ�����ݣ�false ����ʧ�ܣ�������󣬲μ�������
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int timeout [IN]: ���ͽ������ݵĳ�ʱʱ�䣬�Ժ����
   // ����  : int* errcode [OUT]: ���صĴ�����
   // ����  : char * errormessage [OUT]: ���صĴ�����Ϣ
   // Argument   : bool bCheckUserData [IN]: ָ���Ƿ����userdata��飬true-���飬����false(0)�����
   BCCCLT_API  bool BCCallNextEx(BCHANDLE Handle, int timeout, int* errcode,char * errormessage,bool bCheckUserData);
   
   // ����  : ��ѯ��ǰ���ݿ������ݼ�¼����
   // ����  : int �������ݼ�¼���� 0-��׼ST_PACK��¼��1-ST_SDPACK��RawData��¼
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   BCCCLT_API  int BCGetXPackType(BCHANDLE Handle); 
   
   // ����  : ����BCGetXPackType����Ϊ1��ST_SDPACK��ļ�¼��������ñ�������ȡ������Ч��¼�е�RawData��¼
   // ����  : int : >=0 - �ɹ�����RawData�����ݳ���(�ֽ���); -1:RawData Size��С; -2: ��ST_SDPACK���¼���¼������; -3: ��Ч���Handle
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * RawData [OUT]: ��Ŷ�ȡ��RawData���ݿ� 
   // ����  : int RawDataSize [IN]: RawData���ݿ�Ŀɴ�ŵ������
   BCCCLT_API  int BCGetRawRecord(BCHANDLE Handle, int Row, char * RawData, int RawDataSize);

   // ����  : ���Ӧ�������У��Ƿ���ָ���ֶε�ֵ
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char *FieldName [IN]: �ֶ����� ��"vsmess"
   BCCCLT_API  bool BCIsValidField(BCHANDLE Handle, int Row, char *FieldName);

   //////////////////////////////////////////////////////////////////////////
   // ��ȡӦ����и�����¼�е��ֶ�ֵ

   // ����  : ����������ȡ��Row����¼�еĶ�Ӧ�ֶε�ֵ (int)
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ������ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
   // ����  : int* Value [OUT]: ���صõ���ֵ
	BCCCLT_API  bool BCGetIntFieldByName(BCHANDLE Handle,int Row,char * FieldName,int * Value);

   // ����  : ����������ȡ��Row����¼�еĶ�Ӧ�ֶε�ֵ (double)
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ������ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
   // ����  : double* Value [OUT]: ���صõ���ֵ
	BCCCLT_API  bool BCGetDoubleFieldByName(BCHANDLE Handle,int Row,char * FieldName,double * Value);

   // ����  : ����������ȡ��Row����¼�еĶ�Ӧ�ֶε�ֵ (double)
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ������ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
   // ����  : char * Value [OUT]: ���صõ���ֵ
   // ����  : int ValueBufferSize [IN]: Value�������ĳ��ȣ������Դ�ŵ���󳤶ȣ� һ�����Ϊ <=256
	BCCCLT_API  bool BCGetStringFieldByName(BCHANDLE Handle,int Row,char * FieldName,char * Value,int ValueBufferSize);

   // ������: BCGetPCharFieldByName
   // ���  : ������ 2009-5-12 14:33:10
   // ����  : ��BCGetStringFieldByName��ͬ�����ڣ����ֶ�����Ϊunsigned char������� ValueΪԭʼֵ��������0x��ʼ�Ŀɴ�ӡ�ַ���
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Row [IN]: ������ָ���ļ�¼�� [0..RecordCount-1]
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
   // ����  : char * Value [OUT]: ���صõ���ֵ
   // ����  : int ValueBufferSize [IN]: Value�������ĳ��ȣ������Դ�ŵ���󳤶ȣ� һ�����Ϊ <=256
   BCCCLT_API  bool BCGetPCharFieldByName(BCHANDLE Handle,int Row,char * FieldName,char * Value,int ValueBufferSize);

   //////////////////////////////////////////////////////////////////////////
   // 

   // ����  : ��ȡ����ʽ��XPack��������Ч�ֶ�����ֵ
   // ����  : int ����ST_PACK�е������Ч�ֶ�����ֵ
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
	BCCCLT_API  int BCGetMaxColumn(BCHANDLE Handle);

   // ����  : �����ֶ����ƣ�����ֶ�������
   // ����  : int >=0 Ϊ��XPACK����Ч�ֶΣ����ص�ֵ��Ϊ�����ţ�-1Ϊ��XPACK��û������ֶ�; -2: ��Ч���
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : char * FieldName [IN]: �ֶ����� ��"vsmess"
	BCCCLT_API  int BCGetFieldColumn(BCHANDLE Handle, char * FieldName);
   
   // ����  : ȡ�ö�Ӧ�ֶε�����(��BCNewHandle������Ľṹ�����ļ����)
   // ����  : int  : 0 - ���ֶ�; 1-char; 2-vs_char; 3-unsigned char; 4-int; 5-double 
   //                -1 �ֶ����������ڻ�Handle����
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : char * FieldName [IN]: ָ�����ֶ�����,��"vsmess"
   BCCCLT_API  int BCGetFieldTypeByName(BCHANDLE Handle,char * FieldName);

   // ����  : ��ȡָ���ֶε���Ϣ
   // ����  : bool : true-Ϊ��Ч�ֶ�
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int Col [IN]: ָ�����ֶ�������
   // ����  : char * FieldName [OUT]: ����ָ�����ֶ�����
   // ����  : int* FieldType [OUT]: ָ�����ֶ�����
   // ����  : int* FieldLength [OUT]: ָ�����ֶγ���
	BCCCLT_API  bool BCGetFieldInfo(BCHANDLE Handle,int Col, char * FieldName, int * FieldType, int * FieldLength);

   // ����  : ����Ҫ��һ�������ʱ������������Ҫ���Ͷ��¼��ʱ��Ϊ�˻��һ���ܹ����Ͷ��ټ�¼��
   //         ��Ҫ�ڵ�һ����¼��SetxxxxFieldByName�����󣬲ſɵ��ñ�������
   //          ע�⣺���м�¼ʹ�õ��ֶα�����ͬ�������ǵ�һ����¼���õ���Щ�ֶ�
   // ����  : int �������������ö�������¼
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   BCCCLT_API  int BCEmulateMaxRows(BCHANDLE Handle);

   // ����  : �����Ƿ�򿪵��Ը����ļ�
   // ����  : void 
   // ����  : bool bDebug [IN]: 0(false) - off; 1(true) - on
   BCCCLT_API  void BCSetDebugSwitch(bool bDebug);


   // ����  : ����Ϣ��¼��KLG�ļ��У�ֻ����BCSetDebugSwitch(true) ����Ч��
   // ����  : bool  ��BCSetDebugSwitch(true)�Ƿ�д��ɹ�
   // ����  : char *szMsg : ��Ҫ��¼���ı���Ϣ��
   BCCCLT_API bool BCKLGMsg(char *szMsg);

   // ����  : ��ǰ�����ú�XPack�������������з��������ķ����(destno:funcno)�㲥
   //          ע��:���ڽ��ն˿�����࣬����ģʽ��ͬBCC�ܹ��е�������Ϣ0ģʽ����
   //                ���ն˲�Ҫ����Ӧ���ȷ����Ϣ������Ҳ��Ҫ����
   //                ������ֻ����DRTP4�汾��ʵ�֣�DRTP3һ�ŷ���false
   // ����  : bool 
   // ����  : BCHANDLE Handle [IN]: ָ����XPack���������
   // ����  : int drtpno [IN]: ���Ǹ�ֱ����ҵ��ͨѶƽ̨�ڵ�㲥���ݣ��μ�BCAddDrtpNode()�ķ���ֵ
   // ����  : int branchno [IN]: Ŀ��Ӧ�÷������ӵ�ҵ��ͨѶƽ̨�ڵ��� >=0;  =0: ��ȫ���㲥,�������ָ���ڵ��ϵĹ㲥
   // ����  : int function [IN]: Ŀ��Ӧ�÷���ע����ҵ��ͨѶƽ̨�����ڽ��չ㲥�Ĺ��ܺţ�����Ϊͨ�ù��ܺŻ�ר�Ž��չ㲥�Ĺ��ܺţ�
   // ����  : int* errcode [OUT]: ���صĴ�����
   // ����  : char *errmsg [OUT]: ��Ҫ512�ֽڿռ䣬���ش�����Ϣ
   BCCCLT_API bool BCBroad(BCHANDLE Handle, int drtpno, int destno, int funcno, int *errcode, char *errmsg);
   

#ifdef __cplusplus
}
#endif

#undef BCCCLT_API
   

#endif
