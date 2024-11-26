/*
Module : FileTreeCtrl.h
Purpose: Interface for an MFC class which provides a tree control similiar 
         to the left hand side of explorer

Copyright (c) 1999 - 2016 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


////////////////////////////////// Macros / Defines  //////////////////////////

#pragma once

#ifndef __FILETREECTRL_H__
#define __FILETREECTRL_H__

#ifndef FILETREECTRL_EXT_CLASS
#define FILETREECTRL_EXT_CLASS
#endif //#ifndef FILETREECTRL_EXT_CLASS

#ifndef FILETREECTRL_EXT_API
#define FILETREECTRL_EXT_API
#endif //#ifndef FILETREECTRL_EXT_API

#ifndef FILETREECTRL_BASE_CLASS
#define FILETREECTRL_BASE_CLASS CTreeCtrl
#endif //#ifndef FILETREECTRL_BASE_CLASS

#ifndef _In_
#define _In_
#endif //#ifndef _In_

#ifndef _Out_
#define _Out_
#endif //#ifndef _Out_

#ifndef _Inout_
#define _Inout_
#endif //#ifndef _Inout_


/////////////////////////// Includes //////////////////////////////////////////

#ifndef __AFXMT_H__
#pragma message("To avoid this message, put afxmt.h in your pre compiled header (normally stdafx.h)")
#include <afxmt.h>
#endif //#ifndef __AFXMT_H__

#ifndef _SHLOBJ_H_
#pragma message("To avoid this message, put shlobj.h in your pre compiled header (normally stdafx.h)")
#include <shlobj.h>
#endif //#ifndef _SHLOBJ_H_

#ifndef _LM_
#pragma message("To avoid this message, put lm.h in your pre compiled header (normally stdafx.h)")
#include <lm.h>
#endif //#ifndef _LM_

#ifndef __AFXMT_H__
#pragma message("To avoid this message, put afxmt.h in your pre compiled header (normally stdafx.h)")
#include <afxmt.h>
#endif //#ifndef __AFXMT_H__


/////////////////////////// Classes ///////////////////////////////////////////

//Class which gets stored int the item data on the tree control
class FILETREECTRL_EXT_CLASS CFileTreeCtrlItemInfo
{
public:
//Enums
  enum NodeType
  {
    DesktopNode,
    NetworkNode,
    MyDocumentsNode,
    MyComputerNode,
    CurrentUserFolderNode,
    FileNode,
    FolderNode,
    DriveNode,
    UnknownNode
  };

//Constructors / Destructors
  CFileTreeCtrlItemInfo();
  CFileTreeCtrlItemInfo(_In_ const CFileTreeCtrlItemInfo& ItemInfo);
  virtual ~CFileTreeCtrlItemInfo();

//Member variables
  CString       m_sFQPath;          //Fully qualified path for this item
  CString       m_sRelativePath;    //The "Display name" for this item, Normally will be a relative bit of the path from "m_sFQPath"
  NETRESOURCE*  m_pNetResource;     //Used if this item is under Network Neighborhood
  BYTE          m_NodeType;         //The type of node this is (A BYTE and not a NodeType instance to optimize memory footprint)
  bool          m_bExtensionHidden; //Is the extension being hidden for this item
};


//Class which encapsulates access to the System image list which contains
//all the icons used by the shell to represent the file system
class FILETREECTRL_EXT_CLASS CSystemImageList : public CImageList
{
public:
//Constructors / Destructors
  CSystemImageList();

protected:
  friend class CFileTreeCtrl; //Allow the FileTreeCtrl access to our internals
};


//class which manages enumeration of shares. This is used for determining 
//if an item is shared or not
class FILETREECTRL_EXT_CLASS CShareEnumerator
{
public:
//Constructors / Destructors
  CShareEnumerator();
  virtual ~CShareEnumerator();

//Methods
  void Refresh(); //Updates the internal enumeration list
  BOOL IsShared(_In_ const CString& sPath);

protected:
//Data
  SHARE_INFO_502* m_pNTShareInfo; //share info
  DWORD           m_dwShares;     //The number of shares enumerated
};


//Class which implements the tree control representation of the file system
class FILETREECTRL_EXT_CLASS CFileTreeCtrl : public FILETREECTRL_BASE_CLASS
{
public:
//Enums
  enum HideFileExtension
  {
    HideExtension,
    DoNoHideExtension,
    UseTheShellSetting
  };

  enum DriveType
  {
    DRIVE_ATTRIBUTE_REMOVABLE = 0x00000001,
    DRIVE_ATTRIBUTE_FIXED     = 0x00000002,
    DRIVE_ATTRIBUTE_REMOTE    = 0x00000004,
    DRIVE_ATTRIBUTE_CDROM     = 0x00000010,
    DRIVE_ATTRIBUTE_RAMDISK   = 0x00000020
  };

//Constructors / Destructors
  CFileTreeCtrl();

//Public methods
  void              SetRootFolder(_In_ const CString& sPath);
  CString           GetRootFolder() const { return m_sRootFolder; };
  CString           GetSelectedPath();
  HTREEITEM         SetSelectedPath(_In_ const CString& sPath, _In_ BOOL bExpanded = FALSE);
  void              SetShowFiles(_In_ BOOL bFiles);
  BOOL              GetShowFiles() const { return m_bShowFiles; };
  void              SetShowNetwork(_In_ BOOL bShowNetwork);
  BOOL              GetShowNetwork() const { return m_bShowNetwork; };
  void              SetUsingDifferentIconForSharedFolders(_In_ BOOL bShowSharedUsingDifferentIcon);
  BOOL              GetUsingDifferentIconForSharedFolders() const { return m_bShowSharedUsingDifferentIcon; };
  void              SetAllowDragDrop(_In_ BOOL bAllowDragDrop) { m_bAllowDragDrop = bAllowDragDrop; };
  BOOL              GetAllowDragDrop() const { return m_bAllowDragDrop; };
  void              SetAllowRename(_In_ BOOL bAllowRename) { m_bAllowRename = bAllowRename; };
  BOOL              GetAllowRename() const { return m_bAllowRename; };
  void              SetAllowOpen(_In_ BOOL bAllowOpen) { m_bAllowOpen = bAllowOpen; };
  BOOL              GetAllowOpen() const { return m_bAllowOpen; };
  void              SetAllowProperties(_In_ BOOL bAllowProperties) { m_bAllowProperties = bAllowProperties; };
  BOOL              GetAllowProperties() const { return m_bAllowProperties; };
  void              SetAllowDelete(_In_ BOOL bAllowDelete) { m_bAllowDelete = bAllowDelete; };
  BOOL              GetAllowDelete() const { return m_bAllowDelete; };
  void              SetAllowCreateFolder(_In_ BOOL bAllowCreateFolder) { m_bAllowCreateFolder = bAllowCreateFolder; };
  BOOL              GetAllowCreateFolder() const { return m_bAllowCreateFolder; };
  void              SetDriveHideFlags(_In_ DWORD dwDriveHideFlags);
  DWORD             GetDriveHideFlags() const { return m_dwDriveHideFlags; };
  void              SetFileHideFlags(_In_ DWORD dwFileHideFlags);
  DWORD             GetFileHideFlags() const { return m_dwFileHideFlags; };
  void              SetFolderHideFlags(_In_ DWORD dwFolderHideFlags);
  DWORD             GetFolderHideFlags() const { return m_dwFolderHideFlags; };
  INT_PTR           AddMultipleFileNameMasks(_In_ const CString& sFileNameMasks);
  void              SetFileNameMask(_In_ INT_PTR nIndex, _In_ const CString& sFileNameMask);
  void              AddFileNameMask(_In_ const CString& sFileNameMask);
  void              RemoveFileNameMask(_In_ int nIndex) { m_sFileNameMasks.RemoveAt(nIndex); };
  CString           GetFileNameMask(_In_ int nIndex) const { return m_sFileNameMasks.GetAt(nIndex); };
  CString           GetFileNameMasks() const;
  void              SetNetworkItemTypes(_In_ DWORD dwTypes);
  DWORD             GetNetworkItemTypes() const { return m_dwNetworkItemTypes; };
  void              SetShowDriveLabels(_In_ BOOL bShowDriveLabels);
  BOOL              GetShowDriveLabels() const { return m_bShowDriveLabels; };
  COLORREF          GetCompressedColor() const { return m_rgbCompressed; };
  void              SetCompressedColor(_In_ COLORREF rgbCompressed);
  BOOL              GetUsingDifferentColorForCompressed() const { return m_bShowCompressedUsingDifferentColor; };
  void              SetUsingDifferentColorForCompressed(_In_ BOOL bShowCompressedUsingDifferentColor);
  COLORREF          GetEncryptedColor() const { return m_rgbEncrypted; };
  void              SetEncryptedColor(_In_ COLORREF rgbEncrypted);
  BOOL              GetUsingDifferentColorForEncrypted() const { return m_bShowEncryptedUsingDifferentColor; };
  void              SetUsingDifferentColorForEncrypted(_In_ BOOL bShowEncryptedUsingDifferentColor);
  HideFileExtension GetShowFileExtensions() const { return m_FileExtensions; };
  void              SetShowFileExtensions(_In_ HideFileExtension FileExtensions);
  BOOL              GetShowMyComputer() const { return m_bShowMyComputer; };
  void              SetShowMyComputer(_In_ BOOL bShowMyComputer);
  BOOL              GetShowDesktop() const { return m_bShowDesktop; };
  void              SetShowDesktop(_In_ BOOL bShowDesktop);
  BOOL              GetShowMyDocuments() const { return m_bShowMyDocuments; };
  void              SetShowMyDocuments(_In_ BOOL bShowMyDocuments);
  BOOL              GetShowCurrentUserFolder() const { return m_bShowCurrentUserFolder; };
  void              SetShowCurrentUserFolder(_In_ BOOL bShowCurrentUserFolder);
  BOOL              GetShowRootedFolder() const { return m_bShowRootedFolder; };
  void              SetShowRootedFolder(_In_ BOOL bShowRootedFolder);
  void              SetAutoRefresh(_In_ BOOL bAutoRefresh);
  BOOL              GetAutoRefresh() const { return m_nAutoRefresh; };
  virtual CString   ItemToPath(_In_ HTREEITEM hItem) const;
  virtual HTREEITEM PathToItem(_In_ const CString& sPath) const;
  virtual BOOL      IsFile(_In_ HTREEITEM hItem);
  virtual BOOL      IsFolder(_In_ HTREEITEM hItem);
  virtual BOOL      IsDrive(_In_ HTREEITEM hItem);
  virtual BOOL      IsNetworkShare(_In_ HTREEITEM hItem);
  virtual BOOL      IsCompressed(_In_ HTREEITEM hItem);
  virtual BOOL      IsEncrypted(_In_ HTREEITEM hItem);
  virtual BOOL      IsShared(_In_ const CString& sPath);
  static BOOL       IsFile(_In_ const CString& sPath);
  static BOOL       IsFolder(_In_ const CString& sPath);
  static BOOL       IsDrive(_In_ const CString& sPath);
  static BOOL       IsUNCServerName(_In_ const CString& sPath);
  static BOOL       IsCompressed(_In_ const CString& sPath);
  static BOOL       IsEncrypted(_In_ const CString& sPath);
  static BOOL       IsShortcut(_In_ const CString& sFilename);
  static BOOL       IsWindowsVistaOrLater();
  virtual BOOL      Rename(_In_ HTREEITEM hItem);
  virtual BOOL      ShowProperties(_In_ HTREEITEM hItem);
  virtual BOOL      Delete(_In_ HTREEITEM hItem);
  virtual BOOL      Open(_In_ HTREEITEM hItem);
  virtual HTREEITEM CreateFolder(_In_ HTREEITEM hItem, _In_ const CString& sNewFolderName = _T(""));
  virtual void      PopulateTree(); 
  virtual void      UpOneLevel();
  virtual void      Refresh();
  virtual BOOL      GoBack();
  virtual BOOL      CanGoBack() const { return m_PrevItems.GetSize() != 0; };
  virtual BOOL      GoForward();
  virtual BOOL      CanGoForward() const { return m_NextItems.GetSize() != 0; };
  virtual INT_PTR   GetMaxHistory() const { return m_nMaxHistory; };
  virtual void      SetMaxHistory(_In_ INT_PTR nMaxHistory);
  INT_PTR           GetBackSize() const;
  CString           GetBackItemText(_In_ INT_PTR nBack) const;
  INT_PTR           GetForwardSize() const;
  CString           GetForwardItemText(_In_ INT_PTR nForward) const;
  void              CollapseExpandBranch(_In_ HTREEITEM hItem, _In_ int nAction);
  void              CollapseAll();
  void              ExpandAll();
  void              Clear();

//Debug / Assert help
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif //#ifdef _DEBUG

protected:
#ifndef FILETREECTRL_NO_RESOURCES
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg BOOL OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
#endif //#ifndef FILETREECTRL_NO_RESOURCES
  afx_msg void OnProperties();
  afx_msg void OnUpdateProperties(CCmdUI* pCmdUI);
  afx_msg void OnRename();
  afx_msg void OnUpdateRename(CCmdUI* pCmdUI);
  afx_msg void OnOpen();
  afx_msg void OnUpdateOpen(CCmdUI* pCmdUI);
  afx_msg void OnDelete();
  afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUpOneLevel(CCmdUI* pCmdUI);
  afx_msg void OnBack();
  afx_msg void OnUpdateBack(CCmdUI* pCmdUI);
  afx_msg void OnForward();
  afx_msg void OnUpdateForward(CCmdUI* pCmdUI);
  afx_msg void OnUpOneLevel();
  afx_msg void OnRefresh();
  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
  afx_msg void OnDestroy();
  afx_msg void OnCreateFolder();
  afx_msg void OnUpdateCreateFolder(CCmdUI* pCmdUI);
  afx_msg BOOL OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);

  DECLARE_MESSAGE_MAP()

  DECLARE_DYNCREATE(CFileTreeCtrl)

//Methods
  virtual BOOL        PreTranslateMessage(MSG* pMsg);
  virtual void        PreSubclassWindow();
  virtual void        DisplayPath(_In_ const CString& sPath, _In_ HTREEITEM hParent, _In_ BOOL bUseSetRedraw = TRUE, _In_ BOOL bDeleteExistingSiblings = TRUE);
  virtual void        DisplayDrives(_In_ HTREEITEM hParent, _In_ BOOL bUseSetRedraw = TRUE);
  virtual BOOL        DisplayShares(_In_ HTREEITEM hParent, _In_ const CString& sComputer);
  virtual HTREEITEM   DisplayNetworkComputer(_In_ HTREEITEM hParent, _In_ const CString& sComputer);
  virtual int         GetIconIndex(_In_ const CString& sFilename);
  virtual int         GetIconIndex(_In_ HTREEITEM hItem);
  virtual int         GetSelIconIndex(_In_ const CString& sFilename);
  virtual int         GetSelIconIndex(_In_ HTREEITEM hItem);
  virtual int         GetIconIndex(_In_ LPITEMIDLIST lpPIDL);
  virtual int         GetSelIconIndex(_In_ LPITEMIDLIST lpPIDL);
  virtual BOOL        HasGotSubEntries(_In_ const CString& sDirectory);
  virtual BOOL        HasChildWithText(_In_ HTREEITEM hParent, _In_ const CString& sText);
  virtual HTREEITEM   InsertFileItem(_In_ HTREEITEM hParent, _In_ CFileTreeCtrlItemInfo* pItem, _In_ int nOverlayMask, _In_ int nIcon, _In_ int nSelIcon, _In_ BOOL bCheckForChildren);
  virtual HTREEITEM   FindSibling(_In_ HTREEITEM hParent, _In_ const CString& sItem) const;
  virtual BOOL        DriveHasRemovableMedia(_In_ const CString& sPath);
  BOOL                IsExpanded(_In_ HTREEITEM hItem);
#ifndef FILETREECTRL_NO_RESOURCES
  virtual BOOL        IsDropSource(_In_ HTREEITEM hItem);
  BOOL                IsDragging();
  void                EndDragging(_In_ BOOL bCancel);
  virtual HTREEITEM   GetDropTarget(_In_ HTREEITEM hItem);
#endif //#ifndef FILETREECTRL_NO_RESOURCES
  virtual HTREEITEM   CopyItem(_In_ HTREEITEM hItem, _In_ HTREEITEM hNewParent, _In_ HTREEITEM hAfter = TVI_LAST);
  virtual HTREEITEM   CopyBranch(_In_ HTREEITEM hBranch, _In_ HTREEITEM hNewParent, _In_ HTREEITEM hAfter = TVI_LAST);
  virtual BOOL        CanDisplayFile(_In_ const CFileFind& find);
  virtual BOOL        CanDisplayFolder(_In_ const CFileFind& find);
  virtual BOOL        CanDisplayDrive(_In_ const CString& sDrive);
  virtual BOOL        CanDisplayNetworkItem(_In_ CFileTreeCtrlItemInfo* pItem);
  virtual BOOL        CanHandleChangeNotifications(_In_ const CString& sPath);
  static int CALLBACK CompareByFilenameNoCase(_In_ LPARAM lParam1, _In_ LPARAM lParam2, _In_ LPARAM lParamSort);
  virtual void        CreateMonitoringThread(_In_ const CString& sPath);
  static UINT         _MonitoringThread(_In_ LPVOID pParam);
  virtual UINT        MonitoringThread();
  virtual void        KillMonitoringThread();
  virtual BOOL        GetSerialNumber(_In_ const CString& sDrive, _Out_ DWORD& dwSerialNumber);
  virtual BOOL        IsMediaValid(_In_ const CString& sDrive);
  virtual void        OnSelectionChanged(_In_ NM_TREEVIEW* pNMTreeView, _In_ const CString& sPath);
  virtual BOOL        EnumNetwork(_In_ HTREEITEM hParent);
  virtual BOOL        EnumNetwork(_In_ NETRESOURCE* pParentNetResource, _In_ HTREEITEM hParentItem);
  virtual CString     GetDriveLabel(_In_ const CString& sDrive);
  virtual CString     CreateNewChildFolderName(_In_ const CString& sParentFolder);
  CString             GetCorrectedLabel(_In_ CFileTreeCtrlItemInfo* pItem);
  HTREEITEM           FindServersNode(_In_ HTREEITEM hFindFrom) const;
  BOOL                HasPlusButton(_In_ HTREEITEM hItem);
  void                SetHasPlusButton(_In_ HTREEITEM hItem, _In_ BOOL bHavePlus);
  void                DoExpand(_In_ HTREEITEM hItem);
  virtual void        DisplayRootItems();

//Member variables
  CString                                                     m_sRootFolder;
  CString                                                     m_sMyDocumentsPath;
  CString                                                     m_sCurrentUserFolderPath;
  CString                                                     m_sDesktopPath;
  volatile long                                               m_bShowFiles;
  HTREEITEM                                                   m_hRoot;
#ifndef FILETREECTRL_NO_RESOURCES
  HTREEITEM                                                   m_hItemDrag;
  HTREEITEM                                                   m_hItemDrop;
#endif //#ifndef FILETREECTRL_NO_RESOURCES
  HTREEITEM                                                   m_hNetworkRoot;
  HTREEITEM                                                   m_hDesktop;
  HTREEITEM                                                   m_hMyDocumentsRoot;
  HTREEITEM                                                   m_hMyComputerRoot;
  HTREEITEM                                                   m_hCurrentUserFolderRoot;
  HTREEITEM                                                   m_hRootedFolder;
  BOOL                                                        m_bShowMyComputer;
  BOOL                                                        m_bShowDesktop;
  BOOL                                                        m_bShowMyDocuments;
  BOOL                                                        m_bShowCurrentUserFolder;
#ifndef FILETREECTRL_NO_RESOURCES
  CImageList*                                                 m_pilDrag;
#endif //#ifndef FILETREECTRL_NO_RESOURCES
  UINT_PTR                                                    m_nTimerID;
#ifndef FILETREECTRL_NO_RESOURCES
  //HCURSOR                                                     m_DropCopyCursor;
  //HCURSOR                                                     m_NoDropCopyCursor;
  //HCURSOR                                                     m_DropMoveCursor;
  //HCURSOR                                                     m_NoDropMoveCursor;
#endif //#ifndef FILETREECTRL_NO_RESOURCES
  UINT                                                        m_TimerTicks;
  BOOL                                                        m_bAllowDragDrop;
  BOOL                                                        m_bAllowRename;
  BOOL                                                        m_bAllowOpen;
  BOOL                                                        m_bAllowProperties;
  BOOL                                                        m_bAllowDelete;
  BOOL                                                        m_bAllowCreateFolder;
  DWORD                                                       m_dwDriveHideFlags;
  DWORD                                                       m_dwFileHideFlags;
  DWORD                                                       m_dwFolderHideFlags;
  CStringArray                                                m_sFileNameMasks;
  COLORREF                                                    m_rgbCompressed;
  BOOL                                                        m_bShowCompressedUsingDifferentColor;  
  COLORREF                                                    m_rgbEncrypted;
  BOOL                                                        m_bShowEncryptedUsingDifferentColor;  
  CArray<HTREEITEM, HTREEITEM>                                m_PrevItems;
  CArray<HTREEITEM, HTREEITEM>                                m_NextItems;
  INT_PTR                                                     m_nMaxHistory;            
  BOOL                                                        m_bUpdatingHistorySelection;
  BOOL                                                        m_bShowNetwork;
  BOOL                                                        m_bShowSharedUsingDifferentIcon;
  HideFileExtension                                           m_FileExtensions;
  DWORD                                                       m_dwMediaID[26];
  IMalloc*                                                    m_pMalloc; 
  IShellFolder*                                               m_pShellFolder;
  DWORD                                                       m_dwNetworkItemTypes;
  BOOL                                                        m_bShowDriveLabels;
  BOOL                                                        m_bShowRootedFolder;
  LONG volatile                                               m_nAutoRefresh;
#ifndef FILETREECTRL_NO_CACHE_COMPUTER_ICON  
  int                                                         m_nComputerIconIndex;
  int                                                         m_nSelectedComputerIconIndex;
#endif
  CString                                                     m_sMonitoringPath;          //The path we are monitoring
  CWinThread*                                                 m_pMonitoringThread;        //The pointer for the monitor thread
  CEvent                                                      m_MonitoringTerminateEvent; //Event using to terminate the thread
  CSystemImageList                                            m_SystemImageList;          //The one and only system image list instance
  CShareEnumerator                                            m_SharedEnumerator;         //The one and only share enumerator
};

//MFC Data exchange routines
void FILETREECTRL_EXT_API DDX_FileTreeValue(_In_ CDataExchange* pDX, _In_ CFileTreeCtrl& ctrlFileTree, _Inout_ CString& sItem);

#endif //#ifndef __FILETREECTRL_H__
