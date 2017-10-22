extern crate input_stream;

fn main() {
    use input_stream::InputStream;
    use std::thread::Builder;
    use std::io::{stdout, BufReader, Write, BufWriter};
    use std::fs::File;
    use solve::Tree;

    Builder::new()
        .stack_size(32 * 1024 * 1024)
        .spawn(|| {
            let f = File::open("C.in").expect("C.in");
            let mut input = InputStream::new(BufReader::new(f));

            let stdout = stdout();
            let mut writer = BufWriter::new(stdout.lock());
            let vertices = input.scan().expect("N");
            let many_colors = input.scan().expect("M");

            let colors: Vec<_> = (0..vertices)
                .map(|_| input.scan::<usize>().expect("color of vertex"))
                .collect();
            let mut tree = Tree::new(colors);

            for _ in 1..vertices {
                let x: usize = input.scan().expect("edge vertex");
                let y: usize = input.scan().expect("edge vertex");
                tree.add_edge(x - 1, y - 1);
            }

            for chain in tree.get_cover_chains(many_colors) {
                writeln!(writer, "{} {} {}", chain.color(), chain.from() + 1, chain.to() + 1).unwrap();
            }
        })
        .unwrap()
        .join()
        .unwrap();
}

mod solve {
    use std::vec::Vec;

    #[derive(Copy, Clone)]
    struct Ends {
        from: usize,
        to: usize,
    }

    pub struct Chain {
        color: usize,
        ends: Ends,
    }

    impl Chain {
        pub fn color(&self) -> usize {
            self.color
        }

        pub fn from(&self) -> usize {
            self.ends.from
        }

        pub fn to(&self) -> usize {
            self.ends.to
        }
    }

    pub struct Tree {
        colors: Vec<usize>,
        edges: Vec<Vec<usize>>,
    }

    impl Tree {
        pub fn new(colors: Vec<usize>) -> Tree {
            let edges = vec![Vec::new(); colors.len()];
            Tree {
                colors,
                edges,
            }
        }

        pub fn len(&self) -> usize {
            self.colors.len()
        }

        pub fn add_edge(&mut self, x: usize, y: usize) {
            self.edges[x].push(y);
            self.edges[y].push(x);
        }

        pub fn get_cover_chains(&self, many_colors: usize) -> Vec<Chain> {
            let solver = Solver::new(self, many_colors);
            solver.get_cover_chains()
        }

        fn dfs<InFn, OutFn>(&self, vertex: usize, father: Option<usize>, in_fn: &mut InFn, out_fn: &mut OutFn)
            where
                InFn: FnMut(usize, Option<usize>),
                OutFn: FnMut(usize, Option<usize>),
        {
            in_fn(vertex, father);

            for &neighbor in self.edges[vertex].iter() {
                if Some(neighbor) != father {
                    self.dfs(neighbor, Some(vertex), in_fn, out_fn);
                }
            }

            out_fn(vertex, father);
        }

    }

    struct ColorInfo {
        ends: Ends,
        lca: usize,
        visited: bool,
    }

    struct VertexInfo {
        father: Option<usize>,
        depth: usize,
    }

    struct Solver<'a> {
        tree: &'a Tree,
        color_info: Vec<ColorInfo>,
        vertex_info: Vec<VertexInfo>,
        ancestor: DisjointSet,
        order: Vec<usize>
    }

    impl<'a> Solver<'a> {
        pub fn new(tree: &'a Tree, many_colors: usize) -> Solver<'a> {
            let ends = Solver::get_color_ends(tree, many_colors);
            let lcas = Solver::get_lcas(tree, &ends);

            let color_info = ends.into_iter()
                .zip(lcas)
                .map(|(ends, lca)| {
                    ColorInfo { ends, lca, visited : false }
                })
                .collect();

            let vertex_info = Solver::get_vertex_info(tree);

            let ancestor = DisjointSet::new(tree.len());

            Solver {
                tree,
                color_info,
                vertex_info,
                ancestor,
                order: Vec::new(),
            }
        }

        pub fn get_cover_chains(mut self) -> Vec<Chain> {
            // add colors not appearing at all
            let mut color_count = vec![0; self.color_info.len()];
            for &color in &self.tree.colors {
                color_count[color] += 1;
            }

            let mut missing = Vec::new();
            for (color_index, color_count) in color_count.into_iter().enumerate().skip(1) {
                if color_count == 0 {
                    missing.push(color_index);
                    self.color_info[color_index].visited = true;
                }
            }

            for color in 1..self.color_info.len() {
                self.get_color_order(color);
            }

            self.order.iter()
                .chain(missing.iter())
                .map(|&color_index| {
                    Chain {
                        color : color_index,
                        ends: self.color_info[color_index].ends,
                    }
                })

                .rev()
                .collect()
        }

        fn get_color_ends(tree: &Tree, many_colors: usize) -> Vec<Ends> {
            let mut last_out: Vec<Option<usize>> = vec![None; many_colors + 1];
            let mut first_out: Vec<Option<usize>> = vec![None; many_colors + 1];
            let mut last_in: Vec<Option<usize>> = vec![None; many_colors + 1];

            tree.dfs(0, None, &mut |vertex, _| {
                last_in[tree.colors[vertex]] = Some(vertex);
            }, &mut |vertex, _| {
                first_out[tree.colors[vertex]].get_or_insert(vertex);
                last_out[tree.colors[vertex]] = Some(vertex);
            });

            last_out.into_iter().zip(first_out.into_iter().zip(last_in))
                .map(|x| match x {
                    (None, (_, _)) => Ends { from : 0, to : 0 },
                    (Some(x), (Some(y), Some(z))) => {
                        if y == z {
                            Ends { from : x, to : y }
                        } else {
                            Ends { from : y, to : z }
                        }
                    },
                    _ => unreachable!(),
                })
                .collect()
        }

        fn get_lcas(tree: &Tree, ends: &Vec<Ends>) -> Vec<usize> {
            let mut ancestor = DisjointSet::new(tree.len());
            let mut node_query = vec![None; tree.len()];
            for &ends in ends {
                node_query[ends.from] = Some(ends.to);
                node_query[ends.to] = Some(ends.from);
            }

            let mut query_lca: Vec<Option<usize>> = vec![None; tree.len()];

            tree.dfs(0, None, &mut |_, _| {} , &mut |node, father| {
                for &query in &node_query[node] {
                    query_lca[node] = Some(ancestor.anc(query));
                    query_lca[query] = query_lca[node];
                }

                for father in father {
                    ancestor.merge(father, node);
                }
            });

            ends.iter()
                .map(|&ends| query_lca[ends.from].unwrap())
                .collect()
        }

        fn get_vertex_info(tree: &Tree) -> Vec<VertexInfo> {
            let mut father = vec![None; tree.len()];
            let mut depth = vec![0; tree.len()];

            tree.dfs(0, None, &mut |vertex, father_vertex| {
                father[vertex] = father_vertex;
                depth[vertex] = father_vertex.map(|x| depth[x] + 1).unwrap_or(0);
            }, &mut |_, _| {});

            father.into_iter()
                .zip(depth)
                .map(|(father, depth)| VertexInfo { father, depth })
                .collect()
        }

         fn get_color_order(&mut self, color: usize) {
            if self.color_info[color].visited {
                return;
            }

            self.color_info[color].visited  = true;

            let Ends { from : mut x, to : mut y } = self.color_info[color].ends;
            let lca_depth = self.vertex_info[self.color_info[color].lca].depth;

            x = self.ancestor.anc(x);

            loop {
                use std::mem::swap;
                y = self.ancestor.anc(y);

                if self.vertex_info[x].depth > self.vertex_info[y].depth {
                    swap(&mut x, &mut y);
                }

                if self.vertex_info[y].depth < lca_depth {
                    break;
                }

                if self.tree.colors[y] == color {
                    for &father in &self.vertex_info[y].father {
                        self.ancestor.merge(father, y);
                    }
                } else {
                    self.get_color_order(self.tree.colors[y]);
                }

                if y == 0 {
                    break;
                }
            }

            self.order.push(color);
        }
    }

    struct DisjointSet {
        anc: Vec<usize>,
    }

    impl DisjointSet {
        pub fn new(size: usize) -> DisjointSet {
            DisjointSet {
                anc: (0..size).collect(),
            }
        }

        pub fn merge(&mut self, x: usize, y: usize) {
            let x = self.anc(x);
            let y = self.anc(y);
            if x == y {
                return;
            }

            self.anc[y] = x;
        }

        pub fn anc(&mut self, x: usize) -> usize {
            if self.anc[x] == x {
                return x;
            }

            let anc = self.anc[x];
            let anc = self.anc(anc);
            self.anc[x] = anc;
            anc
        }
    }


}
