
select top 100 PGMCode, PGMName, SourceID, PlayDateTime, Tb1.tPlaylistPlayDateTime, ChnID, nDuration, nClipType, nPRI, SourceStrXML, TaskID, Version from T_IncomingTaskPool left join 
(select T0.strPrimaryID,  DATEADD(millisecond , t1.nPGMSchStartTime, t1.tPGMSchPlayDate) as tPlaylistPlayDateTime
from zjtv.dbo.t_BIDMapCompile as T0 right join zjtv.dbo.t_PlaylistCompile as T1 on T0.strPGMGuid = T1.strPGMGuid 
right join zjtv.dbo.t_ProgramSheet as T2 on t2.strPSID = t1.strPSID 
where t2.nState = 2 and strPrimaryID not like '%%-%%' and DATEADD(millisecond, t1.nPGMSchStartTime, t1.tPGMSchPlayDate) !< '2014-04-10' and DATEADD(millisecond, t1.nPGMSchStartTime, t1.tPGMSchPlayDate) !> '2014-04-10') as Tb1 on tb1.strPrimaryID = PGMCode 
where TaskID is null and 
((PlayDateTime !< '2014-04-01 00:00:00' and PlayDateTime !> '2014-04-10 00:00:00') or nPRI >= 1)   order by
 nPRI desc,  Tb1.tPlaylistPlayDateTime, PlayDateTime



select top 100 PGMCode,
select T0.strPrimaryID,  DATEADD(millisecond, t1.nPGMSchStartTime, t1.tPGMSchPlayDate) as tPlaylistPlayDateTime
from zjtv.dbo.t_BIDMapCompile as T0 right join zjtv.dbo.t_PlaylistCompile as T1 on T0.strPGMGuid = T1.strPGMGuid 
right join zjtv.dbo.t_ProgramSheet as T2 on t2.strPSID = t1.strPSID 
where t2.nState = 2 and strPrimaryID not like '%%-%%' and t1.tPGMSchPlayDate < '%s' and t1.tPGMSchPlayDate > '%s'