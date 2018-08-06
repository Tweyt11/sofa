#include <CGALPlugin/AsyncUpdate.h>
#include <sofa/core/ObjectFactory.h>

namespace cgalplugin{

AsyncUpdate::AsyncUpdate()
    :
    m_source(initLink("from", "Input mesh")),
    m_dest(initLink("to", "Input mesh"))
{

}

void AsyncUpdate::init()
{
    std::cout << "Hello" << std::endl ;
}

void AsyncUpdate::reinit()
{

}

void AsyncUpdate::update()
{

}

void AsyncUpdate::draw(const sofa::core::visual::VisualParams* vparams)
{
    if(m_source.empty() || m_dest.empty())
        return ;

    if(m_source->isComponentStateValid()){
        std::cout << "UPDATE FROM PREDECESSOR" << std::endl ;
    }
}


SOFA_DECL_CLASS(AsyncUpdate)

int AsyncUpdateClass = sofa::core::RegisterObject("")
        .add< AsyncUpdate >();

} /// namespace cgalplugin
