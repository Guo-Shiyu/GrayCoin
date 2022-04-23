use std::time::SystemTime;

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

fn display_state(system: &ParticleSystem) {
    let matrix = (0..system.particles.len()).zip(system.particles.iter());
    for (row_pos, particle) in matrix {
        print!("{:?}", particle);
        if (row_pos + 1) % 10 == 0 {
            println![""]
        }
    }
}

fn thread_main(list: &mut [Particle], enclosure_size: usize) {
    for particle in list.iter_mut() {
        particle.rand_move()
    }

    // I guess enclosure_size is to be used as this
    // for i in 0..enclosure_size {
    //     list[i].rand_move()
    // }
}

fn main() {
    println!("Hello, world!");

    let mut system = ParticleSystem::new();
    let mut pool = scoped_threadpool::Pool::new(Particle::NUM_OF_THREADS as u32);

    for slice in system.particles.chunks_mut(Particle::NUM_OF_THREADS) {
        pool.scoped(|scope| {
            scope.execute(move || thread_main(slice, slice.len()));
        });
    }

    display_state(&system)

}
