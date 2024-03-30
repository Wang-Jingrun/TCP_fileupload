#include <utility/ini_file.h>
using namespace yazi::utility;

IniFile::IniFile(const string & filename)
{
    load(filename);
}

string IniFile::trim(string s)
{
    if (s.empty())
    {
        return s;
    }
    s.erase(0, s.find_first_not_of(" \r\n"));
    s.erase(s.find_last_not_of(" \r\n") + 1);
    return s;
}

bool IniFile::load(const string & filename)
{
    m_sections.clear();
    m_filename = filename;
    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        throw std::logic_error("loading ini file failed: " + m_filename);
    }

    string name;
    string line;
    while (std::getline(ifs, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        }
        if (line[0] == '#' || line[0] == ';') // it's comment
        {
            continue;
        }
        if ('[' == line[0]) // it's section
        {
            auto pos = line.find_first_of(']');
            if (pos == std::string::npos)
            {
                throw std::logic_error("invalid section: " + line);
            }
            name = trim(line.substr(1, pos - 1));
            m_sections[name] = Section();
        }
        else // it's key = value
        {
            auto pos = line.find_first_of('=');
            if (pos == std::string::npos)
            {
                throw std::logic_error("invalid option: " + line);
            }
            string key = line.substr(0, pos);
            key = trim(key);
            string value = line.substr(pos + 1);
            value = trim(value);
            m_sections[name][key] = value;
        }
    }
    return true;
}

void IniFile::save(const string & filename)
{
    std::ofstream ofs(filename.c_str());
    if (ofs.fail())
    {
        throw std::logic_error("saving ini file failed: " + m_filename);
    }
    ofs << str();
    ofs.close();
}

string IniFile::str() const
{
    std::stringstream ss;
    for (const auto & section : m_sections)
    {
        ss << "[" << section.first << "]" << std::endl;
        for (const auto & option : section.second)
        {
            ss << option.first << " = " << option.second << std::endl;
        }
        ss << std::endl;
    }
    return ss.str();
}

void IniFile::show() const
{
    std::cout << str();
}

void IniFile::clear()
{
    m_sections.clear();
}

bool IniFile::has(const string & section)
{
    return m_sections.find(section) != m_sections.end();
}

bool IniFile::has(const string & section, const string & key)
{
    auto it = m_sections.find(section);
    if (it == m_sections.end())
    {
        return false;
    }
    return it->second.find(key) != it->second.end();
}

Value & IniFile::get(const string & section, const string & key)
{
    return m_sections[section][key];
}

void IniFile::set(const string & section, const string & key, const Value & value)
{
    m_sections[section][key] = value;
}

void IniFile::remove(const string & section)
{
    m_sections.erase(section);
}

void IniFile::remove(const string & section, const string & key)
{
    auto it = m_sections.find(section);
    if (it == m_sections.end())
    {
        return;
    }
    it->second.erase(key);
}

