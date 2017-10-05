classdef WaypointGraph < handle
    properties
        nodes = containers.Map;
        waypoints = containers.Map;
    end
    methods
        function obj = WaypointGraph(map)
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
                                    node.links(linked_nodeid) = linked_node;
                                end
                            catch err
                                if (strcmp(err.identifier,'MATLAB:badsubscript'))
                                    continue
                                else
                                    rethrow(err)
                                end
                            end
                        end
                        if length(node.links.values()) > 2
                            obj.waypoints(node.id) = Waypoint(node);
                        end
                    end
                end
            end
            values = obj.waypoints.values();
            explored = containers.Map;
            for i = 1:length(values)
                waypoint_links = values(i).node.links.values();
                for j = 1:length(waypoint_links)
                    exploring_links = waypoint_links(j);
                    if ~explored.isKey(exploring_links(1).id)         
                        node_path = []
                        while ~obj.waypoints.isKey(exploring_links(1).id)
                            explored(exploring_links(1).id) = true;
                            node_path(end + 1) = exploring_links(1);
                            exploring_values = exploring_links(1).links.values();
                            for k = 1:length(exploring_values)
                                exploring_values_links = exploring_values(k);
                                if exploring_values_links(1).id ~= exploring_links(1).id
                                    exploring_links = exploring_values;
                                end
                            end
                        end
                        values(i).routes(exploring_links(1).id) = node_path(1:end - 1);
                        obj.waypoints(exploring_links(1).id).routes(values(i).get_id()) = fliplr(node_path(1:end - 1));
                    end
                end
            end
        end
        function disable_waypoints(self, center, radius)
        end
        function r = get_node(self, nodeid)
            r = self.nodes(nodeid);
        end
        function r = best_path(self, start, target)
        end
    end
end

