-- menu.lua

function n_AddLanguage()
	-- MainFrame
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
	PX2_LM:AddItem1("UnDo", "撤销")
	PX2_LM:AddItem1("ReDo", "重复")
	PX2_LM:AddItem1("Copy", "复制")
	PX2_LM:AddItem1("Paste", "粘贴")
	PX2_LM:AddItem1("Clone", "克隆")
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
	
	-- General
	PX2_LM:AddItem1("Create", "创建")
	
	-- BluePrint
	PX2_LM:AddItem1("BluePrint", "蓝图")
	PX2_LM:AddItem1("Compile", "编译")
	PX2_LM:AddItem1("Disconnect", "断开")
	
	-- Scene
	PX2_LM:AddItem1("Scene", "场景")
	
	-- UI
	PX2_LM:AddItem1("UI", "界面")
	PX2_LM:AddItem1("UIFrame", "层")
	PX2_LM:AddItem1("UIFPicBox", "层图片")
	PX2_LM:AddItem1("UIFText", "层文本")
	PX2_LM:AddItem1("UIButton", "按钮")
	PX2_LM:AddItem1("UICheckButton", "选择按钮")
	PX2_LM:AddItem1("UIComboBox", "组合框")
	PX2_LM:AddItem1("UIEditBox", "编辑框")
	PX2_LM:AddItem1("UIPicBox", "图片")
	PX2_LM:AddItem1("UIText", "文本")	
	
	PX2_LM:AddItem1("CreateScriptControllerFromRes", "从资源创建脚本控制器")
	
	PX2_LM:AddItem1("CreateFromRes", "从资源创建")
	
	-- Event
	PX2_LM:AddItem1("Event", "事件")
	PX2_LM:AddItem1("SimuStart", "仿真开始")
	
	-- Edit
	PX2_LM:AddItem1("Geometry", "几何体")
	PX2_LM:AddItem1("Plane", "平面")
	PX2_LM:AddItem1("Box", "正方体")
	PX2_LM:AddItem1("Sphere", "球")
	PX2_LM:AddItem1("Camera", "像机")
	PX2_LM:AddItem1("Light", "灯光")
	PX2_LM:AddItem1("Trigger", "触发器")
	PX2_LM:AddItem1("Terrain", "地形")
	PX2_LM:AddItem1("Sky", "天空")
	PX2_LM:AddItem1("ImportSelectedRes", "导入选择资源")
	PX2_LM:AddItem1("Import", "导入")
	PX2_LM:AddItem1("SceneActor", "场景角色")
	PX2_LM:AddItem1("Effect", "特效")
	PX2_LM:AddItem1("Billboard", "公告板")
	PX2_LM:AddItem1("Particle", "粒子")
	PX2_LM:AddItem1("Beam", "条带")
	PX2_LM:AddItem1("Ribbon", "波带")
	PX2_LM:AddItem1("Sound", "声音")
	PX2_LM:AddItem1("Node", "节点")
	PX2_LM:AddItem1("SkyBox", "天空盒")
	PX2_LM:AddItem1("Controller", "控制器")
	PX2_LM:AddItem1("ColorController", "颜色控制器")
	PX2_LM:AddItem1("BrightnessController", "亮度控制器")
	PX2_LM:AddItem1("AlphaController", "透明控制器")
	PX2_LM:AddItem1("UniformScaleController", "统一缩放控制器")
	PX2_LM:AddItem1("ScaleController", "缩放控制器")
	PX2_LM:AddItem1("RotateController", "旋转控制器")
	PX2_LM:AddItem1("TranslateController", "位移控制器")		
	PX2_LM:AddItem1("ScriptController", "脚本控制器")
	
	PX2_LM:AddItem1("Copy", "复制")
	PX2_LM:AddItem1("Paste", "粘贴")
	PX2_LM:AddItem1("Delete", "删除")
	PX2_LM:AddItem1("DeleteAll", "删除全部")
	
	PX2_LM:AddItem1("ResetPlay", "重置播放")
	PX2_LM:AddItem1("Play", "播放")
	PX2_LM:AddItem1("Pause", "暂停")
	PX2_LM:AddItem1("Reset", "重置")
	
	PX2_LM:AddItem1("FindInProjectTree", "在工程树中查找")
	PX2_LM:AddItem1("FindInResTree", "在资源树中查找")
	
	PX2_LM:AddItem1("MakeCurve", "曲线化")
	
	-- tips
	PX2_LM:AddItem("Notice", "注意！", "Notice!")
	PX2_LM:AddItem("Tip0", "退出？", "Do you want to exist?")
	PX2_LM:AddItem("TipCreateProject", "工程已经存在，无法创建！", "Project is already exist, can't be created!")
	PX2_LM:AddItem("TipNotHasParent", "必须存在父节点！", "There must has a parent node!")
end