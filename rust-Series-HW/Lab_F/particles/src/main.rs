use std::{time::SystemTime};

#[derive(Debug)]
struct Particle {
    x: i32,
    y: i32,
}

impl Particle {
    const NUM_OF_THREADS: usize = 4;

    fn new(x: i32, y: i32) -> Self {
        return Particle { x: x, y: y };
    }
}

impl Particle {
    fn rand_move(&mut self) {
        self.x = (rand::random::<f32>() * 10.0) as i32  % 10;
        self.y = (rand::random::<f32>() * 10.0) as i32  % 10;
    }
}

struct ParticleSystem {
    particles: Vec<Particle>,
}

impl ParticleSystem {
    fn new() -> Self {
        ParticleSystem {
            particles: {
                let mut vec = Vec::new();
                for i in 0..10 {
                    for j in 0..10 {
                        vec.push(Particle::new(i, j));
                    }
                }
                vec
            },
        }
    }
}

impl ParticleSystem {
    fn stir(&mut self) {
        for part in self.particles.iter_mut() {
            part.rand_move()
        }
    }
}

fn display_state(system: &ParticleSystem) {
    // 粒子系统中共有100个粒子， 每十个为一组分到一个线程进行随机移动， 
    // 为了美观显示粒子系统的状态， 一行十个， 共计十行， 
    // (0..system.particles.len())  生成数字range itertor， 范围是0 到最大离子数（100）， 表示第i个粒子的序号
    // system.particles.iter() 生成每一个粒子的迭代器
    // zip方法打包两个迭代器a, b 为元组 (a, b), 也就是 matrix, 
    let matrix = (0..system.particles.len()).zip(system.particles.iter());
    for (row_pos, particle) in matrix { // 迭代打包后的迭代器， 这里分别取出粒子的序号和每个粒子的信息
        print!("{:?}", particle);   // 打印一个粒子的信息
        if (row_pos + 1) % 10 == 0 {    // 每打印十个粒子， 输出一个换行符， 以达到十个一组的的效果
            println![""]
        }
    }
}

fn test() {
    println!("Initialize:");
    let mut system = ParticleSystem::new();
    display_state(&system);

    println!("--------------------------------");

    println!("Stir:");
    system.stir();
    display_state(&system);
}

fn stir_for_a_while(system: &mut ParticleSystem) {
    const MAX_TIME: u64 = 8; // count by second
    let start = SystemTime::now();
    loop {
        system.stir();
        if SystemTime::now().duration_since(start).unwrap().as_secs() >= MAX_TIME {
            break;
        }
    }
}

fn main() {
    println!("Hello, world!");

    // test();

    let mut system = ParticleSystem::new();
    stir_for_a_while(&mut system);
    display_state(&system)
}
