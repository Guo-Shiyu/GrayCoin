# 开发文档
## 类设计:

- Period : enum   
	时间段枚举，取值可以为以下几种    
	- Year
	- Month 
	- Day 
	- Hour 
	- Mintue 
	- Second  

- TimeDiff : alias of time_t  
	表示两个时刻之差，类似于 std::ptrdiff_t  

- TimePoint :class  
	表示一个具体的时刻      
   - member:   
		- inner_: time_t  时间表示  
   
   - method:    
		- year(self) -> size_t
		- month(self) -> size_t
		- day(self) -> size_t
		- hour(self) -> size_t
		- mintue(self) -> size_t
		- second(self) -> size_t

		- fly(self, time_t offset) -> void 
		时间自然流逝 offset 

		- fly(self, Period period, size_t num) -> void    
		时间自然流逝 num 个 period     
		
		- distance(self, TimePoint p) -> TimeDiff   
		返回自身到目标时间点的 偏移量 
		
		- as_str(self, optional\<string\> format) -> string   
		按照format格式 转换时间成为字符串形式  
		
		- operator + (self, TimeDiff dif) -> Self   
		流逝 dif 长的时间  
		
		- operator - (self, TimeDiff dif) -> Self   
		倒流 dif 长的时间     

		- operator < | > (self, TimePoint p) -> bool  
		比较 时间点的大小， 即 inner_ 的大小

		- now() -> Self*  
		依据 clock() 函数的返回值生成 TimePoint  

		- when(int year, int month, int day, int hour, int min, int sec) -> Self* 

- Timer : class, extend TimePoint 定时器类 
	- member: 
		- repeat_ : bool 是否重复 
		- frenquency_ : Period 重复频率  
	
	- method:   
		- alert(self) -> void  
		在触发时调用的绘图函数   

- ClockDial : class 表盘类  
	- member:  
		- clock_: TimePoint 	内置时钟
		- timers_: std::std::priority_queue\<TimePoint\> 定时器队列 	

	- method:  
        - update(self) -> void   
        执行这个时间段的更新逻辑  
        
		- tick(self) -> void  
		内置时钟到下一秒  
		
		- display(self) -> void   
		绘制表盘  

		- make_dial(optional\<TimePoint*\> start)  
		获取一个指向新的表盘的指针


# 注释   
 针对作业要求，有以下说明  
 - 对于要求2， 默认使用系统当前时间, 通过静态函数 now 获取新实例的指针，如需要用户指定时间，可以 使用静态函数 when 获取新实例的指针。

 - 对于要求4， 指定闹钟重复与否通过创建timer时 repeat参数指定， 重复的复读只能为1s/1min/1hour/1day/1month/1year

 - 三个示例  
	- 第一个  
	在25秒内 设置一个每秒重复的闹钟 每当闹钟到达设置的时间，屏幕下方出现五角星以及提示字符  
	
	- 第二个  
	在八十秒内 设置一个一分钟后提醒的不重复的闹钟， 提示字符同上   

	- 第三个   
	倒计时三十秒， 倒计时结束后无事发生。  
	倒计时时时间显示为 1970 年  是因为采用了 time_t 做内置计时器， 倒计时本质上是将 time_t 变量 设置为 POSIX 开始时间 + 设定计时数字， 并且每秒递减
