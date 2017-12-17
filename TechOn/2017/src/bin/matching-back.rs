extern crate bit_vec;
extern crate input_stream;
extern crate itertools;
extern crate rand;

use std::io::{stdin, stdout, BufReader, BufWriter, Read, Write};
use std::fs::File;
use std::env::args;
use std::process;

use input_stream::InputStream;
use input_stream::errors::ResultExt;

use graph::Graph;

fn run() -> input_stream::Result<()> {
    let args: Vec<String> = args().collect();
    let stdin = stdin();
    let stdout = stdout();

    let mut input: InputStream<BufReader<Box<Read>>> =
        InputStream::new(BufReader::new(if args.len() > 1 {
            Box::new(File::open(&args[1]).chain_err(|| "file problem")?)
        } else {
            Box::new(stdin.lock())
        }));
    let mut writer = BufWriter::new(stdout.lock());

    let nodes = input.scan()?;
    let edges = input.scan()?;

    let mut graph = Graph::new(nodes);

    for _ in 0..edges {
        let x: usize = input.scan()?;
        let y: usize = input.scan()?;
        graph.add_edge(x - 1, y - 1);
    }

    let mut extended_graph = graph.clone();
    let mapping = extended_graph.extend_back();

    extended_graph.complement(&mapping);

    let (extra_nodes, extra_edges) = (extended_graph - graph)?;

    writeln!(writer, "{} {}", extra_nodes, extra_edges.len()).unwrap();
    for (from, to) in extra_edges {
        writeln!(writer, "{} {}", from + 1, to + 1).unwrap();
    }
    for mapped_value in mapping {
        write!(writer, "{} ", mapped_value + 1).unwrap();
    }
    writeln!(writer, "").unwrap();
    Ok(())
}

fn main() {
    match run() {
        Ok(()) => {
            process::exit(0);
        }
        Err(err) => {
            eprintln!("{}", err);
            process::exit(1);
        }
    }
}

mod graph {
    use std::ops::{Index, Sub};
    use std::usize;
    use std::cmp::min;

    use input_stream::Result;
    use bit_vec::BitVec;
    use rand::{self, Rng};

    #[derive(Clone)]
    pub struct Graph {
        size: usize,
        edges: Vec<BitVec>,
        max_mapping: usize,
        degrees: Vec<usize>,
    }

    impl Graph {
        pub fn new(size: usize) -> Graph {
            Graph {
                size,
                edges: vec![BitVec::from_elem(size, false); size],
                max_mapping: 0,
                degrees: vec![]
            }
        }

        pub fn size(&self) -> usize {
            self.size
        }

        pub fn add_edge(&mut self, from: usize, to: usize) {
            self.edges[from].set(to, true);
            self.edges[to].set(from, true);
        }

        pub fn has_edge(&self, from: usize, to: usize) -> bool {
            self.edges[from][to]
        }

        pub fn degrees(&self) -> Vec<usize> {
            self.edges
                .iter()
                .map(|bit_vec| bit_vec.iter().filter(|&x| x).count())
                .collect()
        }

        pub fn extend_back(&mut self) -> Vec<usize> {
            self.degrees = self.degrees();
            let mut mapping = Mapping::new(self);
            let degrees = self.degrees();
            let mut possible_mapping = vec![vec![0; self.size()]; self.size()];
            for i in 0..self.size() {
                for j in 0..self.size() {
                    if i == j {
                        if self.size() % 2 == 0 {
                            possible_mapping[i][j] += 1;
                            continue;
                        }
                    }
                    if degrees[i] + degrees[j] >= self.size() {
                        possible_mapping[i][j] += 1;
                    }
                }
            }

            for _ in 0..100 {
                let mut operations = 30000;
                if self.backtrack(&mut possible_mapping, &mut mapping, &mut operations) {
                    eprintln!("");
                    mapping.mapping.resize(self.size(), usize::MAX);
                    return mapping.mapping;
                }
                eprintln!("Maximum matching so far {}", self.max_mapping);
            }
            panic!("Backtrack failed");
        }

        pub fn complement(&mut self, mapping: &Vec<usize>) {
            let solved = vec![vec![false; self.size()]; self.size()];

            for i in 0..self.size() {
                for j in 0..self.size() {
                    if i == j {
                        continue;
                    }

                    if solved[i][j] {
                        continue;
                    }

                    let mut modulo2 = 0;

                    let mut x = i;
                    let mut y = j;
                    let mut k = 0;
                    loop {
                        k += 1;
                        if self.has_edge(x, y) {
                            modulo2 = k % 2;
                        }
                        x = mapping[x];
                        y = mapping[y];
                        if x == i && y == j {
                            break;
                        }
                    }

                    x = i;
                    y = j;
                    k = 0;
                    loop {
                        k += 1;
                        if k % 2 == modulo2 {
                            self.add_edge(x, y);
                        }

                        x = mapping[x];
                        y = mapping[y];
                        if x == i && y == j {
                            break;
                        }
                    }
                }
            }
        }

        fn backtrack(
            &mut self,
            possible_mapping: &mut Vec<Vec<usize>>,
            mapping: &mut Mapping,
            operations: &mut usize,
        ) -> bool {
            *operations -= 1;
            if *operations == 0 {
                return false;
            }

            let nodes: Vec<_> = (0..self.size()).collect();
            //if !self.try_simple_mapping(mapping, &nodes) {
            //    panic!("Shouldn't have gotten to this point");
            //}

            let current_mappings = mapping.mapping.iter().filter(|&&x| x != usize::MAX).count();
            if self.max_mapping < current_mappings {
                self.max_mapping = current_mappings;
            }

            if mapping.completed() {
                eprintln!("Them chains: {:?}", mapping.find_all_chains());
                return true;
            }

            let matching = MatchingGraph::new(possible_mapping);
            let mut buffer = Vec::new();
            let (mandatory, ways) = matching.few_edges(self.size() - current_mappings);

            if mandatory.len() > 0 {
                //mandatory.resize(2, (0, 0));
                let mut bad = false;
                for &(from, mapped_to) in &mandatory {
                    assert!(mapping[from] == usize::MAX);
                    assert!(mapping.reverse_mapping[mapped_to] == usize::MAX);
                    mapping.add_mapping(from, mapped_to);

                    let chain = mapping.find_chain(from);

                    for i in 0..self.size() {
                        possible_mapping[from][i] += 1;
                        possible_mapping[i][mapped_to] += 1;
                    }

                    if chain.len() > 4 {
                        bad = true;
                    }

                    if possible_mapping[from][mapped_to] > 2 {
                        bad = true;
                    }

                    for i in 0..self.size() {
                        for j in 0..self.size() {
                            if possible_mapping[i][j] > 0 {
                                continue;
                            }

                            //if i == j {
                            //    continue;
                            //}

                            if j == chain.first().cloned().unwrap()
                                || i == chain.last().cloned().unwrap()
                            {
                                // special case
                                if mapping.can_add(i, j) && self.try_mapping(mapping, &nodes, i, j) {
                                    continue;
                                }
                            } else {
                                if mapping.can_add(i, j) && self.try_mapping(mapping, &chain, i, j) {
                                    continue;
                                }
                            }

                            possible_mapping[i][j] += 1;
                            buffer.push((i, j));
                        }
                    }

                    if from == mapped_to {
                        for i in 0..self.size() {
                            possible_mapping[i][i] += 1;
                            buffer.push((i, i));
                        }
                    }

                    let start = chain.first().cloned().unwrap();
                    let end = chain.last().cloned().unwrap();
                    for i in 0..self.size() {
                        if possible_mapping[i][start] == 0 && i != end {
                            let end_chain = mapping.find_chain(i);
                            if end_chain.len() + chain.len() > 4 {
                                possible_mapping[i][start] += 1;
                                buffer.push((i, start));
                            }
                        }

                        if possible_mapping[end][i] == 0 && i != start {
                            let start_chain = mapping.find_chain(i);
                            if start_chain.len() + chain.len() > 4 {
                                possible_mapping[end][i] += 1;
                                buffer.push((end, i));
                            }
                        }
                    }
                }

                if !bad {
                    if self.backtrack(possible_mapping, mapping, operations) {
                        return true;
                    }
                }

                    for &(i, j) in &buffer {
                        possible_mapping[i][j] -= 1;
                    }

                    buffer.clear();


                for &(from, mapped_to) in &mandatory {
                    for i in 0..self.size() {
                        possible_mapping[from][i] -= 1;
                        possible_mapping[i][mapped_to] -= 1;
                    }

                    mapping.clear_mapping(from);
                    if *operations == 0 {
                        return false;
                    }
                }

                return false;
            } else {
                let mut did_dumb = false;
                for (from, mapped_to) in ways {
                    if self.degrees[from] == 0 && mapping[from] == usize::MAX && mapping.reverse_mapping[from] == usize::MAX {
                        if did_dumb {
                            continue;
                        }
                        did_dumb = true;
                    }

                    if self.degrees[mapped_to] == 0 && mapping[mapped_to] == usize::MAX && mapping.reverse_mapping[mapped_to] == usize::MAX {
                        if did_dumb {
                            continue;
                        }
                        did_dumb = true;
                    }

                    mapping.add_mapping(from, mapped_to);

                    let chain = mapping.find_chain(from);

                    if chain.len() > 4 {
                        mapping.clear_mapping(from);
                        continue;
                    }

                    for i in 0..self.size() {
                        possible_mapping[from][i] += 1;
                        possible_mapping[i][mapped_to] += 1;
                    }

                    for i in 0..self.size() {
                        for j in 0..self.size() {
                            if possible_mapping[i][j] > 0 {
                                continue;
                            }

                            //if i == j {
                            //    continue;
                            //}

                            if j == chain.first().cloned().unwrap()
                                || i == chain.last().cloned().unwrap()
                            {
                                // special case
                                if mapping.can_add(i, j) && self.try_mapping(mapping, &nodes, i, j) {
                                    continue;
                                }
                            } else {
                                if mapping.can_add(i, j) && self.try_mapping(mapping, &chain, i, j) {
                                    continue;
                                }
                            }

                            possible_mapping[i][j] += 1;
                            buffer.push((i, j));
                        }
                    }

                    if from == mapped_to {
                        for i in 0..self.size() {
                            possible_mapping[i][i] += 1;
                            buffer.push((i, i));
                        }
                    }

                    let start = chain.first().cloned().unwrap();
                    let end = chain.last().cloned().unwrap();
                    for i in 0..self.size() {
                        if possible_mapping[i][start] == 0 && i != end {
                            let end_chain = mapping.find_chain(i);
                            if end_chain.len() + chain.len() > 4 {
                                possible_mapping[i][start] += 1;
                                buffer.push((i, start));
                            }
                        }

                        if possible_mapping[end][i] == 0 && i != start {
                            let start_chain = mapping.find_chain(i);
                            if start_chain.len() + chain.len() > 4 {
                                possible_mapping[end][i] += 1;
                                buffer.push((end, i));
                            }
                        }
                    }

                    if self.backtrack(possible_mapping, mapping, operations) {
                        return true;
                    }

                    for &(i, j) in &buffer {
                        possible_mapping[i][j] -= 1;
                    }

                    buffer.clear();

                    for i in 0..self.size() {
                        possible_mapping[from][i] -= 1;
                        possible_mapping[i][mapped_to] -= 1;
                    }

                    mapping.clear_mapping(from);
                    if *operations == 0 {
                        return false;
                    }
                }
            }
            false
        }

        fn try_mapping(
            &self,
            mapping: &mut Mapping,
            chain: &[usize],
            from: usize,
            to: usize,
        ) -> bool {
            mapping.add_mapping(from, to);

            for &node in chain {
                // we match node to from, now we have to go both back and forth and find if there is a conflict
                let mut current_mask = 1;
                let mut x = node;
                let mut y = from;

                let mut has_edge_mask = 0;
                loop {
                    if self.has_edge(x, y) {
                        has_edge_mask |= current_mask;
                    }

                    x = mapping.reverse_mapping[x];
                    y = mapping.reverse_mapping[y];
                    current_mask ^= 3;

                    if x == usize::MAX || y == usize::MAX {
                        break;
                    }

                    if x == node && y == from {
                        break;
                    }
                }

                current_mask = 2;
                x = mapping.mapping[node];
                y = to;
                loop {
                    if x == usize::MAX || y == usize::MAX {
                        break;
                    }

                    if self.has_edge(x, y) {
                        has_edge_mask |= current_mask;
                    }

                    x = mapping.mapping[x];
                    y = mapping.mapping[y];
                    current_mask ^= 3;

                    if x == usize::MAX || y == usize::MAX {
                        break;
                    }

                    if x == node && y == from {
                        break;
                    }
                }

                if has_edge_mask == 3 {
                    mapping.clear_mapping(from);
                    return false;
                }
            }
            mapping.clear_mapping(from);
            true
        }
    }


    impl Sub for Graph {
        type Output = Result<(usize, Vec<(usize, usize)>)>;

        fn sub(self, other: Graph) -> Self::Output {
            if self.size() < other.size() {
                return Err("New graph is smaller than initial".into());
            }

            let mut new_edges = Vec::new();
            for i in 0..self.size() {
                for j in i..self.size() {
                    if self.has_edge(i, j) && !other.has_edge(i, j) {
                        new_edges.push((i, j))
                    }
                }
            }

            Ok((self.size() - other.size(), new_edges))
        }
    }

    struct Mapping {
        mapping: Vec<usize>,
        reverse_mapping: Vec<usize>,
    }

    impl Mapping {
        fn new(graph: &Graph) -> Mapping {
            Mapping {
                mapping: vec![usize::MAX; graph.size()],
                reverse_mapping: vec![usize::MAX; graph.size()],
            }
        }

        fn add_mapping(&mut self, from: usize, to: usize) {
            self.mapping[from] = to;
            self.reverse_mapping[to] = from;
        }

        fn clear_mapping(&mut self, from: usize) {
            self.reverse_mapping[self.mapping[from]] = usize::MAX;
            self.mapping[from] = usize::MAX;
        }

        fn completed(&self) -> bool {
            self.mapping.iter().all(|&x| x != usize::MAX)
        }

        fn can_add(&self, from: usize, to: usize) -> bool {
            // good thing is we know from -> to is not cycle yet

            let mut x = to;
            let mut length = 0;
            while self.mapping[x] != usize::MAX {
                x = self.mapping[x];
                length += 1;
            }
            if x != from {
                return true;
            }
            length % 4 == 3
        }

        fn find_chain(&self, index: usize) -> Vec<usize> {
            let mut answer = Vec::new();
            let mut x = index;
            while self.reverse_mapping[x] != usize::MAX && self.reverse_mapping[x] != index {
                x = self.reverse_mapping[x];
            }

            let end = x;
            loop {
                answer.push(x);
                if self.mapping[x] == usize::MAX || self.mapping[x] == end {
                    break;
                }
                x = self.mapping[x];
            }
            return answer;
        }

        fn find_all_chains(&self) -> Vec<Vec<usize>> {
            let mut seen = vec![false; self.mapping.len()];
            let mut answer = Vec::new();
            for i in 0..self.mapping.len() {
                if seen[i] || self.mapping[i] == usize::MAX {
                    continue;
                }
                let chain = self.find_chain(i);
                for &x in &chain {
                    seen[x] = true;
                }

                answer.push(chain);
            }
            return answer;
        }
    }

    impl Index<usize> for Mapping {
        type Output = usize;

        fn index(&self, index: usize) -> &usize {
            &self.mapping[index]
        }
    }

    struct MatchingGraph {
        left_edges: Vec<Vec<usize>>,
        right_edges: Vec<Vec<usize>>,
    }

    impl MatchingGraph {
        fn new(possible_mappings: &Vec<Vec<usize>>) -> MatchingGraph {
            let size = possible_mappings.len();
            let mut left_edges = vec![Vec::new(); size];
            let mut right_edges = vec![Vec::new(); size];
            for i in 0..size {
                for j in 0..size {
                    if possible_mappings[i][j] == 0 {
                        left_edges[i].push(j);
                        right_edges[j].push(i);
                    }
                }
            }
            MatchingGraph {
                left_edges,
                right_edges,
            }
        }

        fn size(&self) -> usize {
            self.left_edges.len()
        }

        fn few_edges(self, need_match: usize) -> (Vec<(usize, usize)>, Vec<(usize, usize)>) {
            let (left_match, _) = self.matching();

            if left_match.iter().filter(|&&x| x.is_some()).count() != need_match {
                return (vec![], vec![]);
            }

            let mut left_possible = vec![0; self.size()];
            let mut right_possible = vec![0; self.size()];


            #[derive(Eq, PartialEq)]
            enum EdgePossible {
                Always,
                Maybe,
                No
            }

            let mut directed_graph = DirectedGraph::new(2 * self.size());
            for node in 0..self.size() {
                for &neighbor in &self.left_edges[node] {
                    if left_match[node] == Some(neighbor) {
                        directed_graph.add_arc(neighbor + self.size(), node);
                    } else {
                        directed_graph.add_arc(node, neighbor + self.size());
                    }
                }
            }

            let component = directed_graph.strongly_connected_components();
            let is_edge_possible = |from: usize, to: usize| {
                if component[from] == component[to + self.size()] {
                    EdgePossible::Maybe
                } else if left_match[from] == Some(to) {
                    EdgePossible::Always
                } else {
                    EdgePossible::No
                }
            };

            let mut mandatory = Vec::new();
            for node in 0..self.size() {
                for &neighbor in &self.left_edges[node] {
                    match is_edge_possible(node, neighbor) {
                        EdgePossible::Always => {
                            //return (Vec::new(), vec![(node, neighbor)]);
                            mandatory.push((node, neighbor));
                        },
                        EdgePossible::Maybe => {
                            left_possible[node] += 1;
                            right_possible[neighbor] += 1;
                        },
                        EdgePossible::No => {}
                    }
                }
            }

            if mandatory.len() > 0 {
                return (mandatory, Vec::new());
            }

            let least_amount: usize = min(
                left_possible.iter().filter(|&&x| x > 0).min().cloned().unwrap_or(usize::MAX),
                right_possible.iter().filter(|&&x| x > 0).min().cloned().unwrap_or(usize::MAX),
            );

            enum Side {
                Right(usize),
                Left(usize)
            }

            let mut candidates = Vec::new();
            for i in 0..self.size() {
                if left_possible[i] == least_amount {
                    candidates.push(Side::Left(i));
                }
                if right_possible[i] == least_amount {
                    candidates.push(Side::Right(i));
                }
            }

            let mut rng = rand::thread_rng();
            let mut ways = Vec::new();
            rng.shuffle(&mut candidates);
            match candidates.first().unwrap() {
                &Side::Right(right_node) => {
                    for &left_node in &self.right_edges[right_node] {
                        if is_edge_possible(left_node, right_node) != EdgePossible::No {
                            ways.push((left_node, right_node));
                        }
                    }
                },
                &Side::Left(left_node) => {
                    for &right_node in &self.left_edges[left_node] {
                        if is_edge_possible(left_node, right_node) != EdgePossible::No {
                            ways.push((left_node, right_node));
                        }
                    }
                }
            }

            rng.shuffle(&mut ways);
            (Vec::new(), ways)
        }

        fn matching(&self) -> (Vec<Option<usize>>, Vec<Option<usize>>) {
            let mut left_match = vec![None; self.size()];
            let mut right_match = vec![None; self.size()];

            loop {
                let mut used = vec![false; self.size()];

                let mut any_match = false;

                for i in 0..self.size() {
                    if left_match[i].is_none() {
                        if self.try_match(i, &mut used, &mut left_match, &mut right_match) {
                            any_match = true;
                        }
                    }
                }
                if !any_match {
                    break;
                }
            }
            (left_match, right_match)
        }

        fn try_match(
            &self,
            node: usize,
            used: &mut Vec<bool>,
            left_match: &mut Vec<Option<usize>>,
            right_match: &mut Vec<Option<usize>>,
        ) -> bool {
            if used[node] {
                return false;
            }

            used[node] = true;

            for &neighbor in &self.left_edges[node] {
                match right_match[neighbor] {
                    None => {
                        left_match[node] = Some(neighbor);
                        right_match[neighbor] = Some(node);
                        return true;
                    }
                    Some(next) => if self.try_match(next, used, left_match, right_match) {
                        left_match[node] = Some(neighbor);
                        right_match[neighbor] = Some(node);
                        return true;
                    },
                }
            }
            false
        }
    }

    struct DirectedGraph {
        arcs: Vec<Vec<usize>>
    }

    impl DirectedGraph {
        fn new(size: usize) -> DirectedGraph {
            DirectedGraph {
                arcs: vec![Vec::new(); size]
            }
        }

        fn size(&self) -> usize {
            self.arcs.len()
        }

        fn add_arc(&mut self, from: usize, to: usize) {
            self.arcs[from].push(to);
        }

        fn strongly_connected_components(self) -> Vec<usize> {
            let mut used = vec![false; self.size()];

            let mut buffer = Vec::new();
            for i in 0..self.size() {
                if !used[i] {
                    self.dfs(i, &mut used, &mut buffer);
                }
            }

            let mut components = vec![0; self.size()];
            used = vec![false; self.size()];
            let mut num_components = 0;
            for i in buffer {
                if !used[i] {
                    let mut aux = Vec::new();
                    self.dfs(i, &mut used, &mut aux);
                    for node in aux {
                        components[node] = num_components;
                    }
                    num_components += 1;
                }
            }
            components
        }

        fn dfs(&self, node: usize, used: &mut Vec<bool>, buffer: &mut Vec<usize>) {
            if used[node] {
                return;
            }
            used[node] = true;

            for &neighbor in &self.arcs[node] {
                self.dfs(neighbor, used, buffer);
            }
            buffer.push(node);
        }
    }
}
