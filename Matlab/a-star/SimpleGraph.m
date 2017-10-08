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
                        nodeid = Node.mat2id([i, j]);
                        if obj.nodes.isKey(nodeid)
                            node = obj.nodes(nodeid);
                        else
                            node = Node([i, j]);
                            obj.nodes(nodeid) = node;
                        end
                        for k = 1:length(vect)
                            cv = vect(k, :);
                            i_test = i + cv(1);
                            j_test = j + cv(2);
                            try
                                s = map(i_test, j_test);
                                if s == 0
                                    linked_nodeid = Node.mat2id([i_test, j_test]);
                                    if obj.nodes.isKey(linked_nodeid)
                                        linked_node = obj.nodes(linked_nodeid);
                                    else
                                        linked_node = Node([i_test, j_test]);
                                        obj.nodes(linked_nodeid) = linked_node;
                                    end
                                    node.add_link(linked_node, 1);
                                end
                            catch err
                                if (strcmp(err.identifier,'MATLAB:badsubscript'))
                                    continue
                                else
                                    rethrow(err)
                                end
                            end
                        end
                    end
                end
            end
        end
        function r = get_neighbours(self, nodeid)
            r = self.nodes(nodeid).links;
        end
    end
end
