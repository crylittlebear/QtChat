#pragma once

// 气泡方向
typedef enum { None, Left, Right,} Orientation;

typedef enum {
	Text,							// 普通文字消息
	Audio,							// 语音消息
	Picture,						// 图片消息
	Files,							// 文件传输
} MessageType;

typedef enum {
	Unknown				= 0x00,		// 未知	
	Register			= 0x10,		// 用户注册
	Login				= 0x11,		// 用户登录
	Logout				= 0x12,		// 用户登出
	LoginRepeat			= 0x13,		// 用户重复登录
	UserOnline			= 0x15,		// 用户在线
	UserOffline			= 0x16,		// 用户离线
	UpdateHeadPic		= 0x17,		// 更新头像请求
	AddFriend			= 0x20,		// 添加朋友请求
	AddGroup			= 0x21,		// 添加群组请求
	AddFriendResponse	= 0x22,		// 添加朋友响应
	AddGroupResponse	= 0x23,		// 添加群组相应
	CreateGroup			= 0x25,		// 创建群组
	GetMyFriends		= 0x30,		// 获取用户朋友信息
	GetMyGroup			= 0x31,		// 获取用户群组信息
	RefreshFriends		= 0x35,		// 刷新朋友信息
	RefreshGroup		= 0x36,		// 刷新群组信息
	SendMsg				= 0x40,		// 发送普通信息
	SendGroupMsg		= 0x41,		// 发送群组信息
	SendFile			= 0x42,		// 发送文件
	SendPicture			= 0x43,		// 发送图片
	SendFace			= 0x44,		// 发送表情
	ChangePasswd		= 0x50,		// 修改密码
	DeleteFriend		= 0x55,		// 删除朋友
	DeleteGroup			= 0x56,		// 删除群组
	SendFileOk			= 0x60,		// 发送文件完毕
	GetFile				= 0x65,		// 获取文件
	GetPicture			= 0x66		// 获取图片
} E_MSG_TYPE;

typedef enum {
	ConnectedHost		= 0x01,		// 连接上服务器
	DisconnectedHost	= 0x02,		// 从服务器断开连接
	LoginSuccess		= 0x03,		// 登录成功
	LoginPasswdErr		= 0x04,		// 密码错误
	Online				= 0x05,		// 在线
	Offline				= 0x06,		// 离线
	RegisterOk			= 0x07,		// 注册成功
	RegisterFailed		= 0x08,		// 注册失败
	AddFriendOk			= 0x09,		// 添加朋友成功
	AddFriendFailed		= 0x0a,		// 添加朋友失败
} E_STATUS;