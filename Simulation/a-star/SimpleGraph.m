classdef SimpleGraph < handle
    properties
        nodes = containers.Map;
    end
    methods
        function obj = SimpleGraph(map)
            map_size = size(map);
            vect = [-1, 0; 0, 1; 1, 0; 0, -1];
            for j = 1:map_size(2)
                for i = 1:map_size(1)
                    if map(i, j) == 0
                        id = Node.mat2id([i, j]);
                        linked_nodes = cellstr({});
                        for k = 1:length(vect)
                            cv = vect(k, :);
                            i_test = i + cv(1);
                            j_test = j + cv(2);
                            try
                                s = map(i_test, j_test);
                                if s == 0
                                    linked_nodes{end + 1} = Node.mat2id([i_test, j_test]); %#ok<AGROW>
                                end
                            catch err
                                if (strcmp(err.identifier,'MATLAB:badsubscript'))
                                    continue
                                else
                                    rethrow(err)
                                end
                            end
                        end
                        obj.nodes(id) = linked_nodes;
                    end
                end
            end
        end
        function r = neighbours(self, id)
            r = self.nodes(id);
        end
    end
end