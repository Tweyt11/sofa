#include "DDGLink.h"

namespace sofa
{
namespace core
{
namespace objectmodel
{

BaseDDGLink::BaseDDGLink(const BaseDDGLink::InitDDGLink &init)
    : m_name(init.name),
      m_help(init.help),
      m_group(init.group),
      m_linkedBase(init.linkedBase),
      m_owner(init.owner),
      m_dataFlags(init.dataFlags)
{
    addLink(&inputs);
    addLink(&outputs);
    m_counters.assign(0);

    std::cout << "constructing ddglink with name " << m_name << std::endl;
    if (m_owner)
    {
        std::cout << "adding ddglink with name " << m_name << " to " << m_owner->getName() << std::endl;
        m_owner->addDDGLink(this, m_name);
        std::cout << m_owner->findGlobalDDGLink(m_name).size() << std::endl;
    }
}

BaseDDGLink::~BaseDDGLink()
{

}

void BaseDDGLink::setOwner(Base* owner)
{
    m_owner = owner;
}

void BaseDDGLink::set(Base* linkedBase)
{
    m_linkedBase = linkedBase;
    addInput(&m_linkedBase->d_componentstate);
    ++m_counters[size_t(currentAspect())];
    setDirtyOutputs();
}

void BaseDDGLink::set(const Base* linkedBase)
{
    /// UNSAFE: storing the ptr as non-const.. not a problem when manipulating a DDGLink<T> / DDGLink<const T>
    /// but when manipulating a DDGLink<const T> through its abstract type BaseDDGLink, we must be careful to use
    /// the correct getter or undefined behavior will occur.
    m_linkedBase = const_cast<Base*>(linkedBase);
    linkedBase->addComponentStateOutput(this);
    ++m_counters[size_t(currentAspect())];
    setDirtyOutputs();
}

const Base* BaseDDGLink::get() const
{
    const_cast <BaseDDGLink*> (this)->update();
    return m_linkedBase;
}

Base* BaseDDGLink::get()
{
    update();
    /// Dangerous: this method could have an undefined behavior if the linkedBase is a const_cast'ed pointer!
    /// See BaseDDGLink::set(const Base*)
    return m_linkedBase;
}


void BaseDDGLink::update()
{
    for(DDGLinkIterator it=inputs.begin(); it!=inputs.end(); ++it)
    {
        if ((*it)->isDirty())
        {
            (*it)->update();
        }
    }
    ++m_counters[size_t(currentAspect())];
    cleanDirty();
}

const std::string& BaseDDGLink::getName() const
{
    return m_name;
}

Base* BaseDDGLink::getOwner() const
{
    return m_owner;
}

BaseData* BaseDDGLink::getData() const
{
    return nullptr;
}

std::string BaseDDGLink::getPathName() const
{
    if (!m_owner)
        return getName();

    std::string pathname = m_owner->getPathName();
    return pathname + "." + getName();
}

} // namespace objectmodel
} // namespace core
} // namespace sofa
