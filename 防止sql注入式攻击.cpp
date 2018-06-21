#region 防止sql注入攻击

///判断字符串中是否有sql攻击代码
///传入用户提交数据


public bool ProcessSqlStr(string inputString)
{
	string SqlStr = @"and|or|exec|execute|insert|select|delete|update|alter|create|drop|count|\*|chr|char|
			asc|mid|substring|master|truncate|declare|xp_cmdshell|restore|backip|net+user|net+localgroup+administrators";
	try{
		if((inputString != null) && (inputString != String.Empty)){
			string strRegex = @"\b("+SqlStr+@")\b";
			Regex regex = new Regex(strRegex,regexOptions.IgnoreCase);

			//string s = Regex.math(inputString).value;

			if(true == regex.isMatch(inputString))
				return false;
		}
	}
	catch{
		return false;
	}
	return true;
}

//处理用户提交的请求，校验sql注入式攻击，在页面装置时候运行

public void ProcessRequest()
{
	try{
		string getKeys = "";
		string sqlErrorPage = System.Configuration.Configurationsetting.Appsettings["sqlErrorPage"].toStirng();
		if(System.Web.HttpContext.Current.Request.QuerySetting != null){
			for(int i=0;i<System.Web.HttpContext.Current.Request.QuerySetting.Count;i++){
				getKeys = System.Web.HttpContext.Current,Request.QuerySetting.Keys[i];
				if(!ProcessSqlStr(System.Web.HttpContext.Current.Request.QuerySetting[getKeys])){
					System.Web.HttpContext.Current.Response.Redirect(sqlErrorPage +"?errmsg="+getKeys+"有sql攻击嫌疑");
					System.Web.HttpContext.Web.HttpContext.Current.Response.end();
				}
			}
		}
		if(System.Web.HttpContext.Current.Request.Form != null){
			for(int i = 0;i<System.Web.HttpContext.Current.Request.Form.Count;i++){
				getKeys = System.Web.HttpContext.Current.Request.Form.Keys[i];
				if(!ProcessSqlStr(System.web.HttpContext.Current.Request.Form[getKeys]))
				{
					System.Web.HttpContext.Current.Response.Redirect(sqlErrorPage+"?errmsg="+getKeys+"有SQL攻击嫌疑");
					System.WEb.HttpContext.Current.Response.end();
				}
			}
		}
	}
	catch{
		//错误处理：处理用户提交信息
	}
}
#endregion

#region 转换sql代码

//提取字符固定长度

public string CheckStringLength(string inputString,Int32 maxLength){
	if((inputString != null) && (inputString != String.Empty)){
		inputString = inputString.Trim();

		if(inputString.Length > maxLength)
			inputString = inputString.substring(0,maxLength);
	}
	return inputString;
}

//将输入字符串中的sql敏感字替换成"[敏感字]",输出时替换回来

public string MyEncodeInputString(string inputString){
	//要替换的敏感字
	string SqlStr = @"and|or|exec|execute|insert|select|delete|update|alter|create|drop|count|\*|chr|char|
			asc|mid|substring|master|truncate|declare|xp_cmdshell|restore|backip|net+user|net+localgroup+administrators";
	try{
		if((inputString != null) && (inputString != String.Empty)){
			string strRegex = @"\b("+SqlStr+@")\b";
			Regex regex = new Regex(strRegex,RegexOptions.IgnoreCase);
			MatchCollection matcher = Regex.matches(inputString);
			for(int i=0;i<matcheer.Count;i++)
				inputString = inputString.Replace(matches[i].value,"["+matches[i].value+"]");

		}
	}
	catch{
		return "";
	}	
	return inputString;
}

//将敏感字替换回来
public string MyDecodeOutputString(string outputString){
	string SqlStr = @"and|or|exec|execute|insert|select|delete|update|alter|create|drop|count|\*|chr|char|
			asc|mid|substring|master|truncate|declare|xp_cmdshell|restore|backip|net+user|net+localgroup+administrators";
	try{
		if((outputString != null) && (outputString != String.Empty)){
			String strRegex = @"\[\b("+SqlStr + @"\b\]";
			Regex regex = new Regex(strRegex,RegexOptions.IgnoreCase);
			MatchCollection matches = Regex.Matches(outputString);
			for(int i=0;i<matcher.Count;i++){
				outputString = outputString.Replace(matches[i].value,matches[i].value.substring(1,matches[i].value.Length-2));
			}
		}
	}
	catch{
		return "";
	}
	return outputString;
}

#endregion