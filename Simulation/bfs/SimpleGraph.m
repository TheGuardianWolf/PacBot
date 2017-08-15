classdef SimpleGraph < handle
    properties
        openings = {};
        nodes = containers.Map;
    end
    methods
        function r = neighbours(self, id)
            r = self.nodes(id);
        end
    end
end
