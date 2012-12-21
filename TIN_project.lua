TIN_project = Proto("Tin_project", "TIN_project protocol")
local f = TIN_project.fields

f.msgid = ProtoField.uint8 ("Tin_project.msgid", "Message Id")
f.data = ProtoField.bytes("Tin_project.data", "Data")

function TIN_project.dissector (buffer, pinfo, tree)

	local subtree = tree:add(TIN_project, buffer())
	local offset = 0
	
	local msgid = buffer(offset,1):uint()
	
	pinfo.cols.protocol = "TIN_project"
	pinfo.cols.info = "Message ID: "
	pinfo.cols.info:append(msgid:uint())

	subtree:add(f.msgid, msgid)
	offset = offset + 1
	subtree:add(f.data,buffer(offset))
	
end

function TIN_project.init()
end

tcp_table = DissectorTable.get("tcp.port")
tcp_table:add(8080,TIN_project)
