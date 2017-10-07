classdef Graph < handle
    %ALISTGRAPH Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        nodes;
        edges;
        supernodes;
        superedges;
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

            % Make a copy for supernodes
            supernode_map = map;

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

            % % Mark the supernodes
%             for i = 1:length(self.nodes)
%                 edge_count = length(self.edges{i});
%                 if edge_count > 2
%                     self.supernodes{end + 1} = self.nodes{i};
%                 end
%             end
            % for i = 1:cols
            %     for j = 1:rows
                    

            %     end
            % end

            % % Build the superarcs

        end
        
        function r = grid2nodeid(self, x, y)
            for i = 1:length(self.nodes)
                if self.nodes{i}.pos_grid(1) == x && self.nodes{i}.pos_grid(2) == y
                    r = self.nodes{i}.id;
                    break
                end
            end
        end
    end
    
end

