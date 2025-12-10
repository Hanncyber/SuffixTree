/*
 * SuffixTreeGUI.cpp - Animated visualization of Suffix Tree building process
 * 
 * This program creates a graphical user interface that shows the step-by-step
 * construction of a suffix tree using Ukkonen's algorithm.
 * 
 * Dependencies: SFML (Simple and Fast Multimedia Library)
 * Install: sudo apt-get install libsfml-dev (Ubuntu/Debian)
 *          brew install sfml (macOS)
 * 
 * Compile: g++ -std=c++11 SuffixTreeGUI.cpp SuffixTree.cpp -lsfml-graphics -lsfml-window -lsfml-system -o SuffixTreeAnimation
 * Run: ./SuffixTreeAnimation
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include "SuffixTree.hpp"

// Node position for visualization
struct NodePosition {
    Node* node;
    float x, y;
    int depth;
    NodePosition(Node* n, float _x, float _y, int d) : node(n), x(_x), y(_y), depth(d) {}
};

class SuffixTreeVisualizer {
private:
    SuffixTree* tree;
    sf::RenderWindow window;
    sf::Font font;
    std::vector<NodePosition> nodePositions;
    
    int currentStep;
    int maxSteps;
    std::string inputText;
    bool paused;
    
    // Visual parameters
    const float NODE_RADIUS = 20.0f;
    const float LEVEL_HEIGHT = 100.0f;
    const float MIN_HORIZONTAL_SPACING = 60.0f;
    const sf::Color NODE_COLOR = sf::Color(100, 150, 255);
    const sf::Color EDGE_COLOR = sf::Color(50, 50, 50);
    const sf::Color TEXT_COLOR = sf::Color::White;
    const sf::Color ACTIVE_COLOR = sf::Color(255, 100, 100);
    
public:
    SuffixTreeVisualizer(const std::string& text) 
        : window(sf::VideoMode(1200, 800), "Suffix Tree Animation"),
          currentStep(0), maxSteps(0), inputText(text), paused(true) {
        
        window.setFramerateLimit(2); // 2 frames per second for animation
        
        // Try to load a font (built-in fallback if not available)
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            // Try alternative paths
            if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
                if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
                    std::cerr << "Warning: Could not load font. Text may not display correctly." << std::endl;
                }
            }
        }
        
        // Initialize tree with empty string, will build step by step
        tree = nullptr;
        maxSteps = text.length();
    }
    
    ~SuffixTreeVisualizer() {
        if (tree) delete tree;
    }
    
    void run() {
        // Initialize tree
        tree = new SuffixTree(inputText.c_str());
        
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        sf::Time timePerFrame = sf::seconds(0.5f); // 0.5 seconds per step
        
        while (window.isOpen()) {
            processEvents();
            
            timeSinceLastUpdate += clock.restart();
            
            // Auto-advance animation if not paused
            if (!paused && timeSinceLastUpdate >= timePerFrame) {
                timeSinceLastUpdate = sf::Time::Zero;
                nextStep();
            }
            
            render();
        }
    }
    
private:
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Space:
                        paused = !paused;
                        break;
                    case sf::Keyboard::Right:
                        nextStep();
                        break;
                    case sf::Keyboard::Left:
                        previousStep();
                        break;
                    case sf::Keyboard::R:
                        reset();
                        break;
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    void nextStep() {
        if (currentStep < maxSteps) {
            tree->extend(currentStep);
            currentStep++;
        }
    }
    
    void previousStep() {
        if (currentStep > 0) {
            // Rebuild tree from scratch up to previous step
            currentStep--;
            delete tree;
            tree = new SuffixTree(inputText.c_str());
            for (int i = 0; i < currentStep; i++) {
                tree->extend(i);
            }
        }
    }
    
    void reset() {
        currentStep = 0;
        delete tree;
        tree = new SuffixTree(inputText.c_str());
        paused = true;
    }
    
    void render() {
        window.clear(sf::Color(240, 240, 240));
        
        // Calculate node positions
        calculateNodePositions();
        
        // Draw edges first (so they appear behind nodes)
        drawEdges();
        
        // Draw nodes
        drawNodes();
        
        // Draw UI elements
        drawUI();
        
        window.display();
    }
    
    void calculateNodePositions() {
        nodePositions.clear();
        if (!tree || !tree->root) return;
        
        // Use BFS to assign positions
        std::vector<std::vector<Node*>> levels;
        std::vector<Node*> currentLevel;
        currentLevel.push_back(tree->root);
        
        while (!currentLevel.empty()) {
            levels.push_back(currentLevel);
            std::vector<Node*> nextLevel;
            
            for (Node* node : currentLevel) {
                for (int i = 0; i < 256; i++) {
                    if (node->children[i] != nullptr) {
                        nextLevel.push_back(node->children[i]->dest);
                    }
                }
            }
            currentLevel = nextLevel;
        }
        
        // Calculate positions based on levels
        float windowWidth = static_cast<float>(window.getSize().x);
        
        for (size_t level = 0; level < levels.size(); level++) {
            size_t numNodes = levels[level].size();
            float spacing = windowWidth / (numNodes + 1);
            
            for (size_t i = 0; i < numNodes; i++) {
                float x = spacing * (i + 1);
                float y = 80.0f + level * LEVEL_HEIGHT;
                nodePositions.push_back(NodePosition(levels[level][i], x, y, level));
            }
        }
    }
    
    NodePosition* findNodePosition(Node* node) {
        for (auto& np : nodePositions) {
            if (np.node == node) return &np;
        }
        return nullptr;
    }
    
    void drawEdges() {
        if (!tree || !tree->root) return;
        
        for (auto& parentPos : nodePositions) {
            Node* parent = parentPos.node;
            
            for (int i = 0; i < 256; i++) {
                if (parent->children[i] != nullptr) {
                    Edge* edge = parent->children[i];
                    NodePosition* childPos = findNodePosition(edge->dest);
                    
                    if (childPos) {
                        // Draw edge line
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(parentPos.x, parentPos.y), EDGE_COLOR),
                            sf::Vertex(sf::Vector2f(childPos->x, childPos->y), EDGE_COLOR)
                        };
                        window.draw(line, 2, sf::Lines);
                        
                        // Draw edge label (substring)
                        std::string edgeLabel = getEdgeLabel(edge);
                        sf::Text labelText(edgeLabel, font, 12);
                        labelText.setFillColor(sf::Color::Black);
                        
                        float midX = (parentPos.x + childPos->x) / 2.0f;
                        float midY = (parentPos.y + childPos->y) / 2.0f;
                        labelText.setPosition(midX, midY);
                        window.draw(labelText);
                    }
                }
            }
        }
    }
    
    std::string getEdgeLabel(Edge* edge) {
        if (!edge || !tree) return "";
        
        int start = edge->start;
        int end = *(edge->end);
        
        // Limit label length for readability
        int maxLen = 5;
        int actualLen = end - start + 1;
        
        std::string label;
        for (int i = start; i <= end && i - start < maxLen && i < tree->size; i++) {
            label += tree->text[i];
        }
        
        if (actualLen > maxLen) {
            label += "...";
        }
        
        return label;
    }
    
    void drawNodes() {
        for (auto& np : nodePositions) {
            sf::CircleShape circle(NODE_RADIUS);
            circle.setPosition(np.x - NODE_RADIUS, np.y - NODE_RADIUS);
            
            // Highlight active node
            if (np.node == tree->activeNode) {
                circle.setFillColor(ACTIVE_COLOR);
            } else {
                circle.setFillColor(NODE_COLOR);
            }
            
            circle.setOutlineColor(sf::Color::Black);
            circle.setOutlineThickness(2.0f);
            window.draw(circle);
            
            // Draw suffix index for leaf nodes
            if (np.node->suffixIndex != -1) {
                sf::Text indexText(std::to_string(np.node->suffixIndex), font, 12);
                indexText.setFillColor(sf::Color::White);
                indexText.setPosition(np.x - 8, np.y - 8);
                window.draw(indexText);
            }
        }
    }
    
    void drawUI() {
        // Title
        sf::Text title("Suffix Tree Animation - Ukkonen's Algorithm", font, 24);
        title.setFillColor(sf::Color::Black);
        title.setPosition(10, 10);
        window.draw(title);
        
        // Input text display
        std::stringstream ss;
        ss << "Input: " << inputText;
        sf::Text inputDisplay(ss.str(), font, 16);
        inputDisplay.setFillColor(sf::Color::Black);
        inputDisplay.setPosition(10, 45);
        window.draw(inputDisplay);
        
        // Step counter
        std::stringstream stepSS;
        stepSS << "Step: " << currentStep << " / " << maxSteps;
        sf::Text stepText(stepSS.str(), font, 16);
        stepText.setFillColor(sf::Color::Black);
        stepText.setPosition(window.getSize().x - 150, 10);
        window.draw(stepText);
        
        // Instructions
        std::vector<std::string> instructions = {
            "Controls:",
            "SPACE - Play/Pause",
            "RIGHT - Next Step",
            "LEFT - Previous Step",
            "R - Reset",
            "ESC - Exit"
        };
        
        float yPos = window.getSize().y - 140;
        for (const auto& instruction : instructions) {
            sf::Text text(instruction, font, 14);
            text.setFillColor(sf::Color::Black);
            text.setPosition(10, yPos);
            window.draw(text);
            yPos += 20;
        }
        
        // Status
        std::string status = paused ? "PAUSED" : "PLAYING";
        sf::Text statusText(status, font, 18);
        statusText.setFillColor(paused ? sf::Color::Red : sf::Color::Green);
        statusText.setPosition(window.getSize().x - 120, 40);
        window.draw(statusText);
        
        // Active point info
        if (tree) {
            std::stringstream apSS;
            apSS << "Active Point: ";
            if (tree->activeNode == tree->root) {
                apSS << "root";
            } else {
                apSS << "node";
            }
            apSS << ", edge=" << (tree->activeEdge >= 0 && tree->activeEdge < tree->size ? 
                                   std::string(1, tree->text[tree->activeEdge]) : "none")
                 << ", len=" << tree->activeLength;
            
            sf::Text apText(apSS.str(), font, 14);
            apText.setFillColor(sf::Color::Black);
            apText.setPosition(10, window.getSize().y - 160);
            window.draw(apText);
        }
    }
};

int main() {
    std::cout << "=== Suffix Tree Animation ===" << std::endl;
    std::cout << "This program visualizes the step-by-step construction of a suffix tree." << std::endl;
    std::cout << std::endl;
    
    // Get input from user
    std::string input;
    std::cout << "Enter text to build suffix tree (e.g., 'banana'): ";
    std::getline(std::cin, input);
    
    if (input.empty()) {
        input = "banana"; // Default example
        std::cout << "Using default text: " << input << std::endl;
    }
    
    // Limit input length for visualization purposes
    if (input.length() > 15) {
        std::cout << "Warning: Long input truncated to 15 characters for better visualization." << std::endl;
        input = input.substr(0, 15);
    }
    
    std::cout << std::endl;
    std::cout << "Starting animation..." << std::endl;
    std::cout << "Use the controls shown in the window to navigate the animation." << std::endl;
    
    try {
        SuffixTreeVisualizer visualizer(input);
        visualizer.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
