use std::{
    ops::Add,
    sync::{
        atomic::{AtomicUsize, Ordering},
        Arc,
    },
    time::SystemTime,
};

#[derive(Debug)]
struct Particle {
    x: i32,
    y: i32,
}

impl Particle {
    const NUM_OF_THREADS: usize = 4;

    fn new(x: i32, y: i32) -> Self {
        Particle { x: x, y: y }
    }

    fn collide(&self, other: &Particle) -> bool {
        self.x == other.x && self.y == other.y
    }
}

impl Particle {
    fn rand_move(&mut self) {
        self.x = (rand::random::<f32>() * 10.0) as i32 % 10;
        self.y = (rand::random::<f32>() * 10.0) as i32 % 10;
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

fn thread_main(list: &mut [Particle]) {
    for particle in list.iter_mut() {
        particle.rand_move()
    }

    // I guess enclosure_size is to be used as this
    // for i in 0..enclosure_size {
    //     list[i].rand_move()
    // }
}

fn new_thread_main(list: &mut [Particle]) {
    let mut counter = 0;
    for particle in list.iter() {
        for other in list.iter() {
            if particle.collide(other) {
                counter += 1;
            }
        }
    }
    println!("collide num:{}", counter);
}

fn main() {
    println!("Hello, world!");

    let mut system = ParticleSystem::new();

    let thread_num = Particle::NUM_OF_THREADS as usize;
    let mut pool = scoped_threadpool::Pool::new(thread_num as u32);
    let mut other_pool = scoped_threadpool::Pool::new(1);

    let particle_num = system.particles.len();
    for slice in system.particles.chunks_mut(particle_num / thread_num) {
        pool.scoped(|scope| {
            scope.execute(move || thread_main(slice));
        });
    }

    other_pool.scoped(|scope| {
        scope.execute(|| new_thread_main(system.particles.as_mut_slice()));
    })
    
}
