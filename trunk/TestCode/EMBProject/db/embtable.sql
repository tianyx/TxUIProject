��USE [emb]
GO
/****** Object:  Table [dbo].[T_User]    Script Date: 05/07/2014 13:55:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_User](
	[nId] [bigint] IDENTITY(1,1) NOT NULL,
	[strGuid] [varchar](128) NULL,
	[strCode] [varchar](50) NULL,
	[strName] [nvarchar](50) NULL,
	[strPassword] [varchar](50) NULL,
	[strRegisterSys] [nvarchar](256) NULL,
	[tCreateTime] [datetime] NULL,
 CONSTRAINT [PK_T_User] PRIMARY KEY CLUSTERED 
(
	[nId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_IncomingTaskPoolDetails]    Script Date: 05/07/2014 13:55:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[T_IncomingTaskPoolDetails](
	[PGMCode] [nvarchar](50) NOT NULL,
	[PGMName] [nvarchar](200) NULL,
	[SourceID] [int] NULL,
	[SubmitDateTime] [datetime] NOT NULL,
	[PlayDateTime] [datetime] NOT NULL,
	[ChnID] [nvarchar](50) NOT NULL,
	[nDuration] [int] NOT NULL,
	[nClipType] [int] NOT NULL,
	[nPRI] [int] NULL,
	[SourceStrXML] [nvarchar](max) NOT NULL,
	[TaskID] [nvarchar](50) NULL,
	[Version] [int] NOT NULL,
	[nErrcode] [int] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[T_IncomingTaskPool]    Script Date: 05/07/2014 13:55:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[T_IncomingTaskPool](
	[PGMCode] [nvarchar](50) NOT NULL,
	[PGMName] [nvarchar](200) NULL,
	[SourceID] [int] NULL,
	[SubmitDateTime] [datetime] NOT NULL,
	[PlayDateTime] [datetime] NOT NULL,
	[ChnID] [nvarchar](50) NOT NULL,
	[nDuration] [int] NOT NULL,
	[nClipType] [int] NOT NULL,
	[nPRI] [int] NULL,
	[SourceStrXML] [nvarchar](max) NOT NULL,
	[TaskID] [nvarchar](50) NULL,
	[Version] [int] NOT NULL,
	[nErrcode] [int] NULL,
 CONSTRAINT [PK_T_TaskPool] PRIMARY KEY CLUSTERED 
(
	[PGMCode] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[T_EMBTaskLog]    Script Date: 05/07/2014 13:55:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_EMBTaskLog](
	[strTaskID] [varchar](512) NULL,
	[time] [datetime2](7) NULL,
	[nSvrCode] [nchar](10) NULL,
	[nState] [int] NULL,
	[Remark] [varchar](max) NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_EMBTask]    Script Date: 05/07/2014 13:55:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_EMBTask](
	[nTaskID] [bigint] IDENTITY(1,1) NOT NULL,
	[strTaskID] [nvarchar](512) NOT NULL,
	[strProgramID] [nchar](50) NULL,
	[strTaskName] [varchar](512) NULL,
	[strTaskKind] [varchar](50) NULL,
	[strUserCode] [varchar](50) NULL,
	[strUserName] [varchar](50) NULL,
	[strTaskFrom] [varbinary](50) NULL,
	[nPRI] [int] NULL,
	[tSubmitTime] [datetime] NULL,
	[nState] [int] NOT NULL,
	[nRetry] [int] NULL,
	[strExtendInfoXml] [varchar](max) NULL,
	[nDispatchID] [int] NULL,
	[nActorID] [int] NULL,
	[nErrorCode] [int] NULL,
	[nDeleted] [int] NULL,
	[tTimeStartRun] [datetime] NULL,
	[tTimeEndRun] [datetime] NULL,
	[strTaskSequence] [nvarchar](50) NULL,
 CONSTRAINT [PK_T_EMBTask] PRIMARY KEY CLUSTERED 
(
	[nTaskID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_EMBLog]    Script Date: 05/07/2014 13:55:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[T_EMBLog](
	[time] [datetime2](7) NULL,
	[nSvrCode] [int] NULL,
	[nType] [int] NULL,
	[strRemark] [nvarchar](max) NULL
) ON [PRIMARY]
GO