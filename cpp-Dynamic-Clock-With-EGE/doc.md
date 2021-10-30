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

## 开发者想说的 
我直接开喷了 你受不了就别看
	首先， 我要说多少次， 定时器和闹钟就是一个东西， 时间在计算机内的表示方式是time_t 一个 整数，  表示某个时间点 然后 两个整数做差的结果还是一个整数， 
	
所以 这个整数的意义取决于你如何解释，
	所以 闹钟和定时器是设置一个整数， 既可以当作两个时间点的差值 t1-t2 也可以 是 固定的某个future的t`  这他妈就是一个东西  数字逻辑/硬件咋学的？ 你他妈是不是计科学生 

第二 闹钟/定时器重复与否 每当其触发的时候，就新添加一个同样的定时器进来行了， 是用这个差值加上当前时间点的整数值  

第三 瞪大双眼好好看看里面的类怎么设计的， 和你给我发的那坨屎比一比， 不要求实现的多么优雅，起码代码不是说写成一堆能跑i就行 看着就揪心， 怎么插到我的里面来？？？？ 

第四 下次记得文档写清楚， 工业界开发都按照文档来， 口头通知 = 不存在  至少你他妈让别人给你写作业， 总得要求明确一点把？真是皇帝不急太监急 嗷 你催的倒是挺勤快的 隔一会打断施法一次就跟你打游戏一会断网一次似的 

第五 我求求你好好学吧， 不要上来谈条件的时候趾高气扬， 当最后快要交不上作业的时候态度委婉下来， 我一干活的的都觉得丢人 有这本事/心气， 不自己学出点样子来看看？ 我他妈明早八点的考试， 不去复习和你扯这个我真是傻逼了我  我承认因为自己的失误，第一版中没做到任意时刻定时， 但我后来改了。 但是耽误进度最主要的原因是 你和我沟通不详细， 一没有样例，二没有详细输入输出限制。 你看看这种单子是怎么要求的 https://github.com/ACM-Programming-2021/ICPC-Management-System-2021  你看看你咋说的      

第六 题外话， 最开始我看这个单子不错， 适合练手才接的，但是真的被你的迷之操作看傻了。 三天70你不干 两天八十你不干  现在舒服了？ 当然了， 我作为开发者在这件事情上已经找回来了， 这页就过去了   

