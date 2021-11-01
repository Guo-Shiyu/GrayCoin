# 开发文档

请详细阅读 https://github.com/ACM-Programming-2021/ICPC-Management-System-2021   

## 操作-结果-条件   
 
~~~
    结果, 方括号中的可能情况  
    I   -> info
    E   -> error   
    W   -> warning  

    附加, 方括号之后的内容  
    su  -> successfully 
    fa  -> failed 
~~~

- AddTeam  
    - @Arg  
        - teamname: string
    
    - @Result 
        - info - su - 比赛未开始    
        - error - fa - 比赛开始 
        - error - fa - 队伍重名     

- Start  
    - @Arg  
        - duration: int  
        - problem_count: int  
    
    - @Result  
        - info - su - 比赛未开始  
        - error - fa - 比赛已经开始过    

- Submit   
    - @Arg  
        - problem_name: string  
        - team_name: string     
        - sub_status: enum A | W | R | T   
        - time: size_t 

    - @Comment   
    除了A算作通过以外, 其他均不算, 且保证在所有sub操作中 time 的 出现次序单调递增   

- Flush  
    - @Result  
        - info - su  
    
- Freeze  
    - @Result  
        - info - su - 
        - error - fa - 已封榜但未滚榜   
    
- Scroll  
    - @Result  
        - info - su -   
        有一定输出内容      
        - error - fa - 未封榜   

- Query  
    - @Arg  
        - team_name: string 
        - problem_name: string  
        - status: enum A | W | R | T     
    - @Ret  
        - info - su -   
        有一定输出  
        - error - fa - 队伍不存在  

        - warnning - fa - 已经封榜 

        - some output(not error ) - - 没有满足的查询结果   

    - @Comment
      保证询问中的 problem_name 与 status 合法。


- End  
    - @Ret  
        - info - -    


