#pragma once

// ���ݷ���
typedef enum { None, Left, Right, } Orientation;

typedef enum {
	Text,							// ��ͨ������Ϣ
	Audio,							// ������Ϣ
	Picture,						// ͼƬ��Ϣ
	Files,							// �ļ�����
} MessageType;

typedef enum {
	Unknown = 0x00,		// δ֪	
	Register = 0x10,		// �û�ע��
	Login = 0x11,		// �û���¼
	Logout = 0x12,		// �û��ǳ�
	LoginRepeat = 0x13,		// �û��ظ���¼
	UserOnline = 0x15,		// �û�����
	UserOffline = 0x16,		// �û�����
	UpdateHeadPic = 0x17,		// ����ͷ������
	AddFriend = 0x20,		// �����������
	AddGroup = 0x21,		// ���Ⱥ������
	AddFriendResponse = 0x22,		// ���������Ӧ
	AddGroupResponse = 0x23,		// ���Ⱥ����Ӧ
	CreateGroup = 0x25,		// ����Ⱥ��
	GetMyFriends = 0x30,		// ��ȡ�û�������Ϣ
	GetMyGroups = 0x31,		// ��ȡ�û�Ⱥ����Ϣ
	RefreshFriends = 0x35,		// ˢ��������Ϣ
	RefreshGroup = 0x36,		// ˢ��Ⱥ����Ϣ
	SendMsg = 0x40,		// ������ͨ��Ϣ
	SendGroupMsg = 0x41,		// ����Ⱥ����Ϣ
	SendFile = 0x42,		// �����ļ�
	SendPicture = 0x43,		// ����ͼƬ
	SendFace = 0x44,		// ���ͱ���
	ChangePasswd = 0x50,		// �޸�����
	DeleteFriend = 0x55,		// ɾ������
	DeleteGroup = 0x56,		// ɾ��Ⱥ��
	SendFileOk = 0x60,		// �����ļ����
	GetFile = 0x65,		// ��ȡ�ļ�
	GetPicture = 0x66		// ��ȡͼƬ
} E_MSG_TYPE;

typedef enum {
	ConnectedHot = 0x01,		// �����Ϸ�����
	DisconnectedHost = 0x02,		// �ӷ������Ͽ�����
	LoginSuccess = 0x03,		// ��¼�ɹ�
	LoginPasswdErr = 0x04,		// �������
	Online = 0x05,		// ����
	Offline = 0x06,		// ����
	RegisterOk = 0x07,		// ע��ɹ�
	RegisterFailed = 0x08,		// ע��ʧ��
	AddFriendOk = 0x09,		// ������ѳɹ�
	AddFriendFailed = 0x0a,		// �������ʧ��
	UserNotFound = 0x0b,		// �û�������
} E_STATUS;
