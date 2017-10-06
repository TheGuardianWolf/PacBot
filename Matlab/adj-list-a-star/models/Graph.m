classdef Graph < handle
    %ALISTGRAPH Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        nodes;
        supernodes;
        arcs;
        superarcs;
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
            block_length = self.real_width * self.real_height / (cols * rows);
            hblock_length = block_length / 2;
            
            total_nodes = 0;
            % Reformat the map
            for i = 1:cols
                for j = 1:rows
                    if map(i, j) == 1
                        map(i, j) = Inf;
                        total_nodes = total_nodes + 1;
                    else
                        map(i, j) = NaN;
                    end
                end
            end

            nodes = cell(total_nodes);
            arcs = cell(total_nodes);

            % Make a copy for supernodes
            supernode_map = map;

            % Mark node ids and create nodes
            node_id = 0;
            for i = 1:cols
                for j = 1:rows
                    if map(i, j) == Inf
                        map(i, j) = node_id;
                        self.nodes{node_id + 1} = GraphNode(node_id, 1, j, i, block_length * (j - 1) + hblock_length, block_length * (i - 1) + hblock_length);
                        self.arcs{node_id + 1} = {};
                        node_id = node_id + 1;
                    end
                end
            end

            % Create arcs
            vect = [-1, 0; 0, 1; 1, 0; 0, -1]; % 1 - N; 2 - E; 3 - S; 4 - W;

            for i = 1:length(self.nodes)
                for j = 1:length(vect)
                    v = vect(j, :);
                    i_test = self.nodes(i).grid_y + v(1);
                    j_test = self.nodes(i).grid_x + v(2);
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
                        self.arcs{node_id + 1}{end + 1} = GraphArc(j, 1, 0, s);
                    end
                end
            end

            % % Mark the supernodes and superarcs
            % for i = 1:cols
            %     for j = 1:rows
                    

            %     end
            % end

            % % Build the superarcs

        end
    end
    
end

