#include "Entity.h"

Entity::Entity(void) {
    this->childEntities = new std::vector<Entity*>();
    physicalBody = new PhysicalBody(this, 0, Vector3());
    modelMatrix = new Matrix4();
    modelMatrix->toIdentity();
    shader = nullptr;
    model = nullptr;
    parent = nullptr;
    position = Vector3();
    rotation = Vector3();
    scale = Vector3(1, 1, 1);
    numChildEntities = 0;
    posChanged = true;
    rotChanged = true;
    scaleChanged = true;
    distanceToCamera = 0;
    renderRadius = 1.0f;
    translucent = false;
}

Entity::Entity(const Entity &copy) {
    this->modelMatrix = new Matrix4(*(copy.modelMatrix));
    this->shader = new Shader(*(copy.shader));
    this->parent = new Entity(*(copy.parent));
    this->childEntities = new std::vector<Entity*>(*(copy.childEntities));
    this->model = new Model(*(copy.model));
    this->shader = copy.shader;
    this->physicalBody = new PhysicalBody(*(copy.physicalBody));
    this->position = Vector3(copy.position);
    this->rotation = Vector3(copy.rotation);
    this->scale = Vector3(copy.scale);
    this->numChildEntities = copy.numChildEntities;
    this->posChanged = copy.posChanged;
    this->rotChanged = copy.rotChanged;
    this->scaleChanged = copy.scaleChanged;
    this->distanceToCamera = copy.distanceToCamera;
    this->renderRadius = copy.renderRadius;
    this->translucent = copy.translucent;
}

Entity::Entity(Vector3 position, Vector3 rotation, Vector3 scale) {
    this->childEntities = new std::vector<Entity*>();
    this->position = Vector3(position);
    this->rotation = Vector3(rotation);
    this->scale = Vector3(scale);
    modelMatrix = new Matrix4();
    modelMatrix->toIdentity();
    model = nullptr;
    shader = nullptr;
    parent = nullptr;
    numChildEntities = 0;
    posChanged = true;
    rotChanged = true;
    scaleChanged = true;
    distanceToCamera = 0;
    renderRadius = 1.0f;
    translucent = false;
}

Entity::~Entity(void) {
    if (parent != nullptr) {
        //parent->removeChild(this);
    }
    if (modelMatrix != nullptr) {
        delete modelMatrix;
        modelMatrix = nullptr;
    }
    if (childEntities != nullptr) {
        auto itEnd = childEntities->end();
        for (auto it = childEntities->begin(); it != itEnd; ++it) {
            delete *it;
        }
        childEntities->clear();
        delete childEntities;
        childEntities = nullptr;
    }
    shader = nullptr;
    if (physicalBody != nullptr) {
        delete physicalBody;
        physicalBody = nullptr;
    }
}

Entity &Entity::operator=(const Entity &other) {
    *(this->modelMatrix) = *(other.modelMatrix);
    *(this->model) = *(other.model);
    this->shader = other.shader;
    if (parent != NULL) {
        if (other.parent == NULL) {
            this->parent = NULL;
        } else {
            *(this->parent) = *(other.parent);
        }
    } else {
        if (other.parent != NULL) {
            this->parent = new Entity(*(other.parent));
        }
    }
    *(this->childEntities) = *(other.childEntities);
    *(this->physicalBody) = *(other.physicalBody);
    this->position = Vector3(other.position);
    this->rotation = Vector3(other.rotation);
    this->scale = Vector3(other.scale);
    this->numChildEntities = other.numChildEntities;
    this->posChanged = other.posChanged;
    this->rotChanged = other.rotChanged;
    this->scaleChanged = other.scaleChanged;
    this->distanceToCamera = other.distanceToCamera;
    this->renderRadius = other.renderRadius;
    this->translucent = other.translucent;
    return *this;
}

void Entity::onMouseMoved(Vector2 &position, Vector2 &amount) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseMoved(position, amount);
    }
}

void Entity::onMouseClick(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseClick(button, position);
    }
}

void Entity::onMouseDoubleClick(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseDoubleClick(button, position);
    }
}

void Entity::onMouseButtonDown(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseButtonDown(button, position);
    }
}

void Entity::onMouseButtonUp(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseButtonUp(button, position);
    }
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void Entity::onMouseWheelScroll(int amount) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseWheelScroll(amount);
    }
}

void Entity::onKeyPress(SDL_Keysym key) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onKeyPress(key);
    }
}

void Entity::onKeyDown(SDL_Keysym key) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onKeyDown(key);
    }
}

void Entity::onKeyUp(SDL_Keysym key) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onKeyUp(key);
    }
}


void Entity::calculateModelMatrix(Vector3 addPos, Vector3 addRot, Vector3 addSiz, bool pDiff, bool rDiff, bool sDiff) {
    if (posChanged) {
        // Position changed here or in the parent, update it in the matrix.
        modelMatrix->setPositionVector(position + addPos);
        pDiff = true;
        posChanged = false;
    }
    if (rotChanged || scaleChanged) {
        // Rotation or Scale changed, the whole matrix needs to be recalculated.
        Vector3 rot = (rotation + addRot);
        Matrix4 rotationMatrix =
            Matrix4::Rotation(rot.x, Vector3(1, 0, 0)) *
            Matrix4::Rotation(rot.y, Vector3(0, 1, 0)) *
            Matrix4::Rotation(rot.z, Vector3(0, 0, 1));
        *modelMatrix = Matrix4::Translation(position + addPos) * rotationMatrix * Matrix4::Scale(scale * addSiz);
        rDiff = true;
        sDiff = true;
        rotChanged = false;
        scaleChanged = false;
    }
    // Do the same for all the children
    if (numChildEntities > 0) {
        for(auto it = childEntities->begin(); it != childEntities->end(); it++) {
            (*it)->calculateModelMatrix(position + addPos, rotation + addRot, scale * addSiz, pDiff, rDiff, sDiff);
        }
    }
}

void Entity::update(float millisElapsed) {
    // Only update the matrix if this entity isn't a child, because that function also update all children's matrices.
    if (parent == nullptr) {
        calculateModelMatrix(Vector3(), Vector3(), Vector3(1, 1, 1), false, false, false);
    }
    // Update the distanceToCamera if that's changed
    if (Naquadah::getInstance()->getCurrentScene()->getCamera()->hasChanged()) {
        Camera *camera = Naquadah::getInstance()->getCurrentScene()->getCamera();
        Vector3 dir = position - camera->getPosition();
        distanceToCamera = Vector3::dot(dir, dir);
    }
    if (numChildEntities > 0) {
        for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); it++) {
            (*it)->update(millisElapsed);
        }
    }
}

void Entity::draw(float millisElapsed) {
    if (model != nullptr) {
        Naquadah::getInstance()->getCurrentScene()->useShader(shader);
        Naquadah::getRenderer()->updateShaderMatrix("modelMatrix", modelMatrix);
        model->draw();
    }
    if (numChildEntities > 0) {
        for (auto it = childEntities->begin(); it != childEntities->end(); it++) {
            (*it)->draw(millisElapsed);
        }
    }
}

void Entity::addChild(Entity *child) {
    if (child != nullptr) {
        childEntities->push_back(child);
        numChildEntities++;
        child->parent = this;
    }
}

void Entity::removeChild(Entity *child) {
    if (child != nullptr) {
        childEntities->erase(std::remove(childEntities->begin(), childEntities->end(), child), childEntities->end());
        numChildEntities--;
        child->parent = nullptr;
    }
}

void Entity::makeOrphan() {
    if (parent != nullptr) {
        parent->removeChild(this);
    }
}

std::vector<Entity*> Entity::getAllChildren(Entity *entity) {
    std::vector<Entity*> children = std::vector<Entity*>();
    children.emplace_back(entity);
    int size = (int) entity->childEntities->size();
    for (int i = 0; i < size; i++) {
        if ((unsigned) entity->childEntities != 0xcdcdcd01 && (unsigned) entity->childEntities != 0xfeeefeee) {
            std::vector<Entity*> grandChildren = Entity::getAllChildren((*(entity->childEntities))[i]);
            children.insert(children.end(), grandChildren.begin(), grandChildren.end());
        }
    }
    return children;
}

void Entity::setModel(Model *model) {
    if (this->model != nullptr && this->model != model) {
        ResourcesManager::releaseResource(this->model->getName());
    }
    this->model = model;
    if (this->model != nullptr) {
        this->model->addUser();
    }
}