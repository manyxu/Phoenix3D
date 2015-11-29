-- menu.lua

function naAddLanguage()
	-- menus
	PX2_LM:AddItem1("File", "文件")
	PX2_LM:AddItem1("NewProject", "新建工程")
	PX2_LM:AddItem1("NewScene", "新建场景")
	PX2_LM:AddItem1("Open", "打开")
	PX2_LM:AddItem1("Save", "保存")
	PX2_LM:AddItem1("SaveAs", "另存为")
	PX2_LM:AddItem1("Close", "关闭")
	PX2_LM:AddItem1("Exit", "退出")
	PX2_LM:AddItem1("Edit", "编辑")
	PX2_LM:AddItem1("View", "视图")
	
	PX2_LM:AddItem1("Edit", "编辑")
	PX2_LM:AddItem1("OnDo", "撤销")
	PX2_LM:AddItem1("ReDo", "重复")
	PX2_LM:AddItem1("Copy", "复制")
	PX2_LM:AddItem1("Paste", "粘贴")
	PX2_LM:AddItem1("Delete", "删除")
	PX2_LM:AddItem1("DeleteAll", "删除所有")
	PX2_LM:AddItem1("Import", "导入")
	PX2_LM:AddItem1("Export", "导出")
	PX2_LM:AddItem1("Select", "选择")
	PX2_LM:AddItem1("Translate", "位移")
	PX2_LM:AddItem1("Rotate", "旋转")
	PX2_LM:AddItem1("Scale", "缩放")
	PX2_LM:AddItem1("PlayAndStop", "播放/停止")
	PX2_LM:AddItem1("ResetPlay", "重置播放")
	
	PX2_LM:AddItem1("Local", "本地")
	PX2_LM:AddItem1("World", "世界")
	PX2_LM:AddItem1("Parent", "父对象")
	
	PX2_LM:AddItem1("Window", "窗口")
	PX2_LM:AddItem1("Project", "工程")
	PX2_LM:AddItem1("Stage", "舞台")
	PX2_LM:AddItem1("ResView", "资源")
	PX2_LM:AddItem1("Inspector", "属性")
	PX2_LM:AddItem1("Console", "控制台")
	PX2_LM:AddItem1("PreView", "预览")
	
	PX2_LM:AddItem1("Debug", "调试")
	
	PX2_LM:AddItem1("Tool", "工具")
	
	PX2_LM:AddItem1("Help", "帮助")	
	PX2_LM:AddItem1("About", "关于")
	
	-- tips
	PX2_LM:AddItem("Notice", "注意！", "Notice!")
	PX2_LM:AddItem("Tip0", "退出？", "Do you want to exist?")
	PX2_LM:AddItem("TipCreateProject", "工程已经存在，无法创建！", "Project is already exist, can't be created!")
end