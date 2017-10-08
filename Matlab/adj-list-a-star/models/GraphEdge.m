classdef GraphEdge < handle
    %GRAPHNODE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        id;
        a1; % Node id and heading pair
        a2;
    end
    
    methods
        function obj = GraphEdge(id, n1, h1, n2, h2)
            obj.id = id;
            obj.a1 = [n1, h1];
            obj.a2 = [n2, h2];
        end
        
        function [destination, heading] = get_arc_from(self, node_id)
            if self.a1(1) == node_id
                destination = self.a2(1);
                heading = self.a2(2);
            else
                destination = self.a1(1);
                heading = self.a1(2);
            end
        end
        
        function [destination, heading] = get_arc_to(self, node_id)
            if self.a1(1) == node_id
                destination = self.a1(1);
                heading = self.a1(2);
            else
                destination = self.a2(1);
                heading = self.a2(2);
            end
        end
    end
    
end


