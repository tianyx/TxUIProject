select top 1 PGMCode, PGMName, SourceID, PlayDateTime, ChnID, nDuration, nClipType, nPRI, SourceStrXML, TaskID, Version from T_IncomingTaskPool 
where TaskID is null and ((PlayDateTime > '2014-03-01 00:00:00' and PlayDateTime < '2014-05-01 00:00:00') or nPRI >= 100 )
order by  nPRI desc, PlayDateTime 

