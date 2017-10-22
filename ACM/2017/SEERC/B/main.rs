extern crate input_stream;

use input_stream::InputStream;
use std::io::BufReader;
use std::fs::File;

const MODULO: u32 = 1_000_000_007;

fn main() {
    let mut input = InputStream::new(BufReader::new(File::open("B.in").expect("B.in")));

    let slots: usize = input.scan().expect("Number of slots");
    let bricks_falling = input.scan().expect("Bricks falling");

    let mut bricks_at = vec![0; slots + 1];
    for _ in 0..bricks_falling {
        let brick_location: usize = input.scan().expect("Brick location");
        bricks_at[brick_location] += 1;
    }

    let mut bricks_partial_sum = 0;
    let mut last_dp = 1;

    let mut last_for = vec![0; slots + 1];
    last_for[0 - bricks_partial_sum] = 1;


    for i in 1..(slots + 1) {
        if bricks_at[i] > 0 {
            last_dp = 0;
        }

        bricks_partial_sum += bricks_at[i];
        if bricks_partial_sum > i {
            continue;
        }

        last_dp = (last_dp + last_for[i - bricks_partial_sum]) % MODULO;
        last_for[i - bricks_partial_sum] = last_dp;
    }

    println!("{}", last_dp);
}
