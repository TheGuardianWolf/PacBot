classdef Graph < handle
    %ALISTGRAPH Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        nodes;
        edges;
        detatched_edges;
        unique_edges;
        real_width; % mm
        real_height; % mm
    end
    
    methods
        function obj = Graph(map, real_width, real_height)
            obj.real_width = real_width;
            obj.real_height = real_height;
            obj.build(map);
        end

        function build(self, map)
            [cols, rows] = size(map);
            block_length = sqrt(self.real_width * self.real_height / (cols * rows));
            hblock_length = block_length / 2;
            
            total_nodes = 0;
            % Reformat the map
            for i = 1:cols
                for j = 1:rows
                    if map(i, j) == 0
                        map(i, j) = Inf;
                        total_nodes = total_nodes + 1;
                    else
                        map(i, j) = NaN;
                    end
                end
            end

            self.nodes = cell(1, total_nodes);
            self.edges = cell(1, total_nodes);

            % Mark node ids and create nodes
            node_id = 1;
            for i = 1:cols
                for j = 1:rows
                    if map(i, j) == Inf
                        map(i, j) = node_id;
                        self.nodes{node_id} = GraphNode(node_id, j, i, block_length * (j - 1) + hblock_length, block_length * (i - 1) + hblock_length);
                        self.edges{node_id} = {};
                        node_id = node_id + 1;
                    end
                end
            end

            % Create arcs
            vect = [-1, 0; 0, -1 ; 1, 0; 0, 1]; % 1 - N; 2 - W; 3 - S; 4 - E;
            edge_id = 1;
            for i = 1:length(self.nodes)
                for j = 4:-1:3
                    v = vect(j, :);
                    i_test = self.nodes{i}.pos_grid(2) + v(1);
                    j_test = self.nodes{i}.pos_grid(1) + v(2);
                    try
                        s = map(i_test, j_test);
                    catch err
                        if (strcmp(err.identifier,'MATLAB:badsubscript'))
                            break
                        else
                            rethrow(err)
                        end
                    end
                    if ~isnan(s)
                        edge = GraphEdge(edge_id, i, j, s, j - 2);
                        self.edges{i}{end + 1} = edge;
                        self.edges{s}{end + 1} = edge;
                        edge_id = edge_id + 1;
                    end
                end
            end
            
            self.unique_edges = edge_id - 1;
        end
        
        function r = grid2nodeid(self, x, y)
            for i = 1:length(self.nodes)
                if self.nodes{i}.pos_grid(1) == x && self.nodes{i}.pos_grid(2) == y
                    r = self.nodes{i}.id;
                    break
                end
            end
        end
        
        function [x, y] = nodeid2grid(self, id)
            pos = self.nodes{id}.pos_grid;
            x = pos(1);
            y = pos(2);
        end
        
        function r = node_order(self, node_id)
            r = length(self.edges{node_id});
        end
        
        function r = node_is_odd(self, node_id)
            r = mod(length(self.edges{node_id}), 2) == 1;
        end
        
        function edge_remove(self, edge)
            edge_index = zeros(1, 2);
            node_ids = [edge.a1(1), edge.a2(1)];
            
            for i = 1:2
                n = node_ids(i);
                new_edges_size = 0;
                new_edges = cell(1, length(self.edges{n}) - 1);
                for j = 1:length(self.edges{n})
                    if self.edges{n}{j}.id ~= edge.id
                        new_edges_size = new_edges_size + 1;
                        new_edges{new_edges_size} = self.edges{n}{j};
                    else
                        edge_index(i) = j;
                    end
                end
                self.edges{n} = new_edges;
            end
            
            self.detatched_edges{end + 1} = {edge_index, edge};
        end
        
        function edge_attach(self, detatched_edge)
            edge_index = detatched_edge{1};
            node_ids = [detatched_edge{2}.a1(1), detatched_edge{2}.a2(1)];
            
            for i = 1:2
                n = node_ids(i);
                new_edges_size = 0;
                new_edges = cell(1, length(self.edges{n}) + 1);
                for j = 1:(edge_index(i)-1)
                    new_edges_size = new_edges_size + 1;
                    new_edges{new_edges_size} = self.edges{n}{j};
                end
                new_edges_size = new_edges_size + 1;
                new_edges{new_edges_size} = detatched_edge{2};
                for j = (edge_index(i)+1):length(new_edges)
                    new_edges_size = new_edges_size + 1;
                    new_edges{new_edges_size} = self.edges{n}{j-1};
                end
            end
            
            new_detatched_edges_size = 0;
            new_detatched_edges = cell(1, length(self.detatched_edges));
            for i = 1:length(self.detatched_edges)
                if self.detatched_edges{i}{2}.id ~= detatched_edge{2}.id
                    new_detatched_edges_size = new_detatched_edges_size + 1;
                    new_detatched_edges{new_detatched_edges_size} = self.detatched_edges{i};
                end
            end
        end
    end
    
end

